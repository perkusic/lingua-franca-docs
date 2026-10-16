#!/usr/bin/env python3
"""Replace section-sign cross-references (§N, §N.M) in the two Lingua Franca HTMLs with words.

  tutorial edition:  §N -> Lesson N          §N.M -> section N.M
  reference edition: §N -> section N         §N.M -> section N.M
"section" is capitalized at the start of a sentence or block. Homogeneous lists of links
("§4.10 and §4.12") become "sections 4.10 and 4.12", with each number still a link.
Links keep their href; only the text changes. Code comments (<pre>) and SVG text get the same words,
without links. The banner labels "§ 01 / motivation" of the reference edition are left alone
(the sign there is followed by a space, not a number).
Usage: python3 sectwords.py --lessons tutorial.html   |   python3 sectwords.py reference.html
"""
import re, sys

NB = "&nbsp;"
A = r'<a href="#s(\d+)(?:-(\d+))?">&sect;(\d+(?:\.\d+)?)</a>'
SEP = r'(?:, | and | or |&ndash;|, and )'
BLOCK = re.compile(r"<(?:p|li|td|th|figcaption|div|br|h\d|ol|ul|tr|blockquote)\b[^>]*>\s*$")

def sentence_start(s, i):
    pre = s[max(0, i - 600):i]
    if BLOCK.search(pre):
        return True
    txt = re.sub(r"<[^>]+>", "", pre)
    return bool(re.search(r"[.!?][)”’]?\s+$|&rdquo;\s+$", txt)) and not re.search(r"\b(e\.g|i\.e|cf|vs|al)\.\s+$", txt)

def word(top, lessons, cap, plural=False):
    if top and lessons:
        return "Lessons" if plural else "Lesson"
    w = "sections" if plural else "section"
    return w.capitalize() if cap else w

def convert(s, lessons):
    n = {"list": 0, "single": 0, "plain": 0}
    # lists of two or more linked references
    def r_list(m):
        refs = list(re.finditer(A, m.group(0)))
        tops = {r.group(2) is None for r in refs}
        if len(tops) != 1:
            return m.group(0)          # mixed lesson/section list: handled one by one below
        top = tops.pop()
        n["list"] += 1
        body = re.sub(A, lambda r: f'<a href="#s{r.group(1)}' + (f'-{r.group(2)}' if r.group(2) else "") + f'">{r.group(3)}</a>', m.group(0))
        return word(top, lessons, sentence_start(s, m.start()), plural=True) + NB + body
    s = re.sub(rf"{A}(?:{SEP}{A})+", r_list, s)
    def r_one(m):
        n["single"] += 1
        top = m.group(2) is None
        href = f'#s{m.group(1)}' + (f'-{m.group(2)}' if m.group(2) else "")
        return f'<a href="{href}">{word(top, lessons, sentence_start(s, m.start()))}{NB}{m.group(3)}</a>'
    s = re.sub(A, r_one, s)
    def r_plain(m):
        n["plain"] += 1
        top = "." not in m.group(1)
        return word(top, lessons, False) + " " + m.group(1)
    s = re.sub(r"&sect;(\d+(?:\.\d+)?)", r_plain, s)
    return s, n

if __name__ == "__main__":
    lessons = "--lessons" in sys.argv
    for fn in [a for a in sys.argv[1:] if not a.startswith("--")]:
        s = open(fn, encoding="utf-8").read()
        s2, n = convert(s, lessons)
        open(fn, "w", encoding="utf-8", newline="").write(s2)
        left = re.findall(r"&sect;\S{0,6}", s2)
        print(fn, n, "| remaining section signs:", len(left), sorted(set(left))[:5])
