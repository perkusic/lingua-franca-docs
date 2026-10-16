#!/usr/bin/env python3
"""Give every <table> of a project HTML a stable id and a numbered caption, "Table N — title.",
in document order, and add the caption style next to the figcaption style.
Usage: python3 tablecaps.py file.html id1:"title 1" id2:"title 2" ...   (one pair per table, in order)
Refuses to run on a file that already has a <caption>, or when the number of pairs differs from
the number of tables."""
import re, sys

CSS = ('caption{caption-side:top;text-align:left;font-family:"Space Grotesk",sans-serif;font-size:13.5px;'
       'color:var(--muted);padding:0 2px 8px}\ncaption b{color:var(--ink);font-weight:500}\n')

def apply(s, pairs):
    if "<caption" in s:
        sys.exit("already has captions")
    tabs = list(re.finditer(r"<table\b([^>]*)>", s))
    if len(tabs) != len(pairs):
        sys.exit(f"{len(tabs)} tables but {len(pairs)} titles")
    out, pos = [], 0
    for k, (m, (tid, title)) in enumerate(zip(tabs, pairs), 1):
        if "id=" in m.group(1):
            sys.exit(f"table {k} already has an id")
        out += [s[pos:m.start()], f'<table id="{tid}"{m.group(1)}><caption><b>Table {k} &mdash; {title}.</b></caption>']
        pos = m.end()
    out.append(s[pos:])
    s = "".join(out)
    a = "figcaption b{color:var(--ink);font-weight:500}\n"
    if s.count(a) != 1:
        sys.exit("figcaption style not found once")
    return s.replace(a, a + CSS)

if __name__ == "__main__":
    fn = sys.argv[1]
    pairs = [tuple(p.split(":", 1)) for p in sys.argv[2:]]
    s = open(fn, encoding="utf-8").read()
    open(fn, "w", encoding="utf-8", newline="").write(apply(s, pairs))
    print(fn, len(pairs), "tables captioned")
