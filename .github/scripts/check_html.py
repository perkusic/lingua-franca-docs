#!/usr/bin/env python3
"""Structural checks of the published HTMLs: duplicate ids, internal links without a target,
figures without a stable id, and the reference list (every [n] cited resolves, every reference is cited).
Also: no section sign before a number (cross-references are written "section N.M", or "Lesson N" for a
lesson of the tutorial, never "\u00a7N.M"); the banner labels "\u00a7 01 / ..." have a space and pass.
Usage: python3 check_html.py [--no-figure-ids] [--allow-section-sign] file.html [...]   exit 1 on any problem.
--no-figure-ids skips the figure-id rule (the HW-RTI specification has no stable figure ids yet);
--allow-section-sign skips the section-sign rule (the specification keeps its own \u00a7 convention)."""
import re, sys
from collections import Counter

args = sys.argv[1:]
need_fig_ids = "--no-figure-ids" not in args
no_sect = "--allow-section-sign" not in args
args = [a for a in args if a not in ("--no-figure-ids", "--allow-section-sign")]
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
    sect = re.findall(r"(?:&sect;|&#167;|&#[xX]0*[aA]7;|\u00a7)\d[\d.]*", s) if no_sect else []
    problems = []
    if dup: problems.append(f"duplicate ids: {dup}")
    if bad: problems.append(f"internal links without a target: {bad}")
    if nofig: problems.append(f"{nofig} <figure> without a stable id")
    if sect: problems.append(f"{len(sect)} section-sign reference(s), write 'section N.M' instead: {sorted(set(sect))[:5]}")
    if refs and refs - cited: problems.append(f"references never cited: {sorted(refs - cited)}")
    print(f"{fn}: {len(ids)} ids, {nlinks} internal links, {len(refs)} references"
          + ("" if not problems else " | " + " | ".join(problems)))
    ok &= not problems
print("PASS" if ok else "FAIL")
sys.exit(0 if ok else 1)
