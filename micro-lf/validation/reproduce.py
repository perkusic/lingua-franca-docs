#!/usr/bin/env python3
"""Build the generated variants and capture per-federate logs on Linux."""
import argparse
import json
import re
import shutil
import subprocess
import tempfile
import time
from pathlib import Path


CASES = [("V1", 30, False, 1), ("V2", 30, True, 1), ("V3", 100, True, 1)]
CASES += [(f"W{w}", w, True, 3) for w in (55, 59, 60, 61, 62, 65, 70)]


def run(executables, logdir, stagger):
    logdir.mkdir(parents=True, exist_ok=True)
    processes = []
    handles = []
    try:
        for name, path in executables:
            handle = (logdir / f"{name}.log").open("wb")
            handles.append(handle)
            processes.append(subprocess.Popen([str(path)], stdout=handle, stderr=subprocess.STDOUT))
            if stagger and name == "fusion":
                time.sleep(0.3)
        deadline = time.monotonic() + 15
        statuses = [p.wait(timeout=max(0.1, deadline - time.monotonic())) for p in processes]
    except subprocess.TimeoutExpired:
        statuses = [p.poll() for p in processes]
    finally:
        for p in processes:
            if p.poll() is None:
                p.kill()
            p.wait()
        for handle in handles:
            handle.close()
    return statuses


def generated(compiler, temp, evidence, case, wait, slow):
    root = temp / case
    (root / "src").mkdir(parents=True)
    original = Path(__file__).with_name("Validate3.ulf").read_text(encoding="utf-8")
    text, count = re.subn(r"@maxwait\(30 ms\)", f"@maxwait({wait} ms)", original)
    if count != 1:
        raise ValueError("Expected exactly one baseline @maxwait(30 ms)")
    if slow:
        text, count = re.subn(r"^  sb = new Sensor\(period = 150 ms\)$",
                              "  sb = new Sensor(period = 150 ms, slow_at = 3, slow_by = 60 ms)",
                              text, flags=re.M)
        if count != 1:
            raise ValueError("Expected one unmodified sensor B instantiation")
    source = root / "src" / "Validate3.ulf"
    source.write_text(text, encoding="utf-8")
    with (evidence / f"{case}-build.log").open("wb") as log:
        status = subprocess.run([str(compiler), str(source)], cwd=root, stdout=log,
                                stderr=subprocess.STDOUT, timeout=180).returncode
    if status:
        print((evidence / f"{case}-build.log").read_text(errors="replace")[-6000:], flush=True)
    for file in root.rglob("*.c"):
        if "src-gen" in file.parts or "fed-gen" in file.parts:
            if "reactor-uc" in file.relative_to(root).parts or "CMakeFiles" in file.parts:
                continue
            dest = evidence / "generated-c" / case / file.relative_to(root)
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(file, dest)
    if status:
        raise RuntimeError(f"uLFC/native build failed in {case}: exit {status}")
    programs = []
    for name in ("fusion", "sa", "sb"):
        matches = [p for p in (root / "bin").glob("*")
                   if p.is_file() and re.search(rf"(?:^|_){name}(?:_\d+)?$", p.name)]
        if len(matches) != 1:
            raise RuntimeError(f"Cannot identify {name} executable: {list((root / 'bin').glob('*'))}")
        programs.append((name, matches[0]))
    return programs


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("mode", choices=("manual", "trace", "generated"))
    parser.add_argument("input", type=Path)
    parser.add_argument("evidence", type=Path)
    args = parser.parse_args()
    evidence = (args.evidence / args.mode).resolve()
    evidence.mkdir(parents=True, exist_ok=True)
    failures = []
    with tempfile.TemporaryDirectory() as temp:
        for case, wait, slow, repetitions in (CASES[2:3] if args.mode == "trace" else CASES):
            if args.mode == "generated":
                programs = generated(args.input.resolve(), Path(temp), evidence, case, wait, slow)
            else:
                programs = [(name, args.input.resolve() / binary) for name, binary in
                            (("fusion", f"fusion_w{wait}"), ("sa", "sensor_a"),
                             ("sb", "sensor_b_slow" if slow else "sensor_b"))]
            for attempt in range(1, repetitions + 1):
                logdir = evidence / f"{case}-{attempt}"
                statuses = run(programs, logdir, args.mode != "generated")
                fusion = (logdir / "fusion.log").read_text(errors="replace")
                lines = [line for line in fusion.splitlines() if line.startswith("fusion:")]
                result = {"case": case, "run": attempt, "exit": statuses, "fusion": lines}
                print(json.dumps(result), flush=True)
                if statuses != [0, 0, 0] or not any(line.endswith(": PASS") for line in lines):
                    failures.append(f"{case}-{attempt}")
    if failures:
        print(f"Unsuccessful runs: {', '.join(failures)}", flush=True)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
