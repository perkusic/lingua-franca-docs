#!/usr/bin/env python3
"""Rebuild the two HTMLs from skeleton/ and svg/ and check them against the hashes in manifest.json.

Usage:
  python3 assemble.py --check            rebuild in memory, compare with manifest hashes (and with the
                                          canonical HTMLs in the parent folder, if present); exit 1 on any mismatch
  python3 assemble.py --out DIR          also write the rebuilt HTMLs into DIR (never over the canonical files)
Reads files as bytes and writes them as bytes: no newline, encoding or whitespace normalization.
"""
import argparse, hashlib, json, os, re, sys

HERE = os.path.dirname(os.path.abspath(__file__))
PH = re.compile(rb"<!--@svg:([a-z0-9-]+)@-->")

def sha(b): return hashlib.sha256(b).hexdigest()

def svg_bytes(entry, doc):
    if entry["shared"]:
        f, h = entry["file"], entry["sha256"]
    else:
        f, h = entry["variants"][doc]["file"], entry["variants"][doc]["sha256"]
    b = open(os.path.join(HERE, f), "rb").read()
    return b, sha(b) == h, f

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--out")
    a = ap.parse_args()
    m = json.load(open(os.path.join(HERE, "manifest.json"), encoding="utf-8"))
    figs = {e["id"]: e for e in m["figures"]}
    ok = True
    for doc, d in m["documents"].items():
        skel = open(os.path.join(HERE, d["skeleton"]), "rb").read()
        if sha(skel) != d["skeleton_sha256"]:
            print(f"{doc}: skeleton changed since extraction"); ok = False
        bad_svg = []
        def sub(mm):
            b, good, f = svg_bytes(figs[mm.group(1).decode()], doc)
            if not good: bad_svg.append(f)
            return b
        out = PH.sub(sub, skel)
        h = sha(out)
        same = h == d["sha256"]
        canon = os.path.join(HERE, "..", doc)
        canon_note = ""
        if os.path.exists(canon):
            ch = sha(open(canon, "rb").read())
            canon_note = "canonical file identical" if ch == h else f"canonical file DIFFERS ({ch[:16]})"
            ok &= ch == h
        print(f"{doc}: rebuilt {len(out)} bytes sha256 {h[:16]} | manifest {d['sha256'][:16]} | "
              f"{'IDENTICAL' if same else 'MISMATCH'} | {canon_note}" + (f" | changed SVGs: {bad_svg}" if bad_svg else ""))
        ok &= same and not bad_svg
        if a.out:
            os.makedirs(a.out, exist_ok=True)
            dst = os.path.join(a.out, doc)
            if os.path.abspath(dst) == os.path.abspath(canon):
                sys.exit("refusing to overwrite the canonical file")
            open(dst, "wb").write(out)
    print("PASS" if ok else "FAIL")
    sys.exit(0 if ok else 1)

if __name__ == "__main__":
    main()
