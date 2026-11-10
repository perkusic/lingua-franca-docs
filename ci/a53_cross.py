#!/usr/bin/env python3
"""Generate the validation federates with the pinned uLFC, cross-compile each generated
project for aarch64 (the ZCU106 A53 cores), and run them under qemu-aarch64.

Usage: a53_cross.py <reproduce.py dir> <lfc> <toolchain file> <evidence dir>
Logical results are compared with the reference outcomes of reproduce.py; lags under
emulation are not board measurements.
"""
import json, re, subprocess, sys, tempfile
from pathlib import Path

rdir, lfc, toolchain, evidence = (Path(a).resolve() for a in sys.argv[1:5])
sys.path.insert(0, str(rdir))
import reproduce as R  # noqa: E402

QEMU_LIB = "/usr/aarch64-linux-gnu"
evidence.mkdir(parents=True, exist_ok=True)
failures, summary = [], []

def cross_build(case_root, case):
    built = {}
    for inc in sorted(case_root.rglob("Include.cmake")):
        src = inc.parent
        if "reactor-uc" in src.parts or "CMakeFiles" in src.parts:
            continue
        b = src.parent / "build-aarch64"
        log = evidence / f"{case}-cross-{src.parent.name}.log"
        with log.open("wb") as h:
            r1 = subprocess.run(["cmake", "-S", str(src), "-B", str(b), "-DCMAKE_BUILD_TYPE=Release",
                                 f"-DCMAKE_TOOLCHAIN_FILE={toolchain}"], stdout=h, stderr=subprocess.STDOUT)
            r2 = subprocess.run(["cmake", "--build", str(b), "--parallel", "4"], stdout=h, stderr=subprocess.STDOUT) if r1.returncode == 0 else r1
        if r2.returncode:
            raise RuntimeError(f"cross build failed for {src}; see {log.name}")
        for f in b.iterdir():
            if f.is_file() and f.stat().st_mode & 0o111:
                info = subprocess.run(["file", str(f)], capture_output=True, text=True).stdout
                if "ARM aarch64" in info:
                    built[f.name] = (f, src, info.split(":", 1)[1].strip())
    return built

for case, wait, slow, _ in R.CASES:
    reps = 3 if case in ("V1", "V2", "V3") else 1
    with tempfile.TemporaryDirectory() as t:
        native = R.generated(lfc, Path(t), evidence, case, wait, slow)
        built = cross_build(Path(t) / case, case)
        programs = []
        for name, npath in native:
            if npath.name not in built:
                raise RuntimeError(f"no aarch64 binary named {npath.name}: {sorted(built)}")
            arm, src, info = built[npath.name]
            wrapper = Path(t) / f"run_{npath.name}.sh"
            wrapper.write_text(f"#!/bin/sh\nexec qemu-aarch64 -L {QEMU_LIB} {arm}\n")
            wrapper.chmod(0o755)
            programs.append((name, wrapper))
            if case == "V1":
                (evidence / f"binary-{name}.txt").write_text(f"{npath.name}: {info}\nsource: {src.relative_to(Path(t))}\n")
        for attempt in range(1, reps + 1):
            logdir = evidence / f"{case}-{attempt}"
            statuses = R.run(programs, logdir, False)
            fusion = (logdir / "fusion.log").read_text(errors="replace")
            ok = R.matches_reference(fusion, wait, slow)
            lines = [l for l in fusion.splitlines() if l.startswith("fusion:")]
            rec = {"case": case, "run": attempt, "exit": statuses, "reference_match": ok, "fusion": lines}
            print(json.dumps(rec), flush=True)
            summary.append(rec)
            if case in ("V1", "V2", "V3") and (statuses != [0, 0, 0] or not ok):
                failures.append(f"{case}-{attempt}")

(evidence / "summary.json").write_text(json.dumps(summary, indent=1))
print("failures:", failures or "none")
sys.exit(1 if failures else 0)
