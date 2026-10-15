#!/usr/bin/env python3
"""Structural checks of the published HTMLs: duplicate ids, internal links without a target,
figures without a stable id, and the reference list (every [n] cited resolves, every reference is cited).
Usage: python3 check_html.py [--no-figure-ids] file.html [...]   exit 1 on any problem.
--no-figure-ids skips the figure-id rule (the HW-RTI specification has no stable figure ids yet)."""
import re, sys
from collections import Counter

args = sys.argv[1:]
need_fig_ids = "--no-figure-ids" not in args
args = [a for a in args if a != "--no-figure-ids"]
ok = True
for fn in args:
    s = open(fn, encoding="utf-8").read()
    body = re.sub(r"<script\b.*?</script>|<style\b.*?</style>", "", s, flags=re.S)
    ids = re.findall(r'\sid="([^"]+)"', body)
    dup = sorted(i for i, c in Counter(ids).items() if c > 1)
    idset = set(ids)
    bad = sorted({h for h in re.findall(r'href="#([^"]+)"', body) if h not in idset})
    nofig = len(re.findall(r"<figure>", body)) if need_fig_ids else 0
    nlinks = len(re.findall(r'href="#', body))
    cited = set(re.findall(r'href="#(ref\d+)"', body))
    refs = set(re.findall(r'<li id="(ref\d+)"', body))
    problems = []
    if dup: problems.append(f"duplicate ids: {dup}")
    if bad: problems.append(f"internal links without a target: {bad}")
    if nofig: problems.append(f"{nofig} <figure> without a stable id")
    if refs and refs - cited: problems.append(f"references never cited: {sorted(refs - cited)}")
    print(f"{fn}: {len(ids)} ids, {nlinks} internal links, {len(refs)} references"
          + ("" if not problems else " | " + " | ".join(problems)))
    ok &= not problems
print("PASS" if ok else "FAIL")
sys.exit(0 if ok else 1)
