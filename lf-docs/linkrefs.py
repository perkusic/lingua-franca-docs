#!/usr/bin/env python3
"""Turn cross-references in the two Lingua Franca HTMLs into links.

Adds ids:  every <h3> "N.M — ..." gets id="sN-M" (the id the contents sidebar would give it);
           every <figure> gets the stable id of figures/manifest.json (in document order).
Links:     §N, §N.M, Section/section N(.M), Lesson(s) N, Figure(s) N, Example N.M
           (Example only where the target block exists, i.e. in the tutorial).
Untouched: text inside <a>, <svg>, <script>, <style>, <code>, <pre>, <h1>-<h4>, <title>, <span class="msg">,
           attribute values, and the label "Figure N &mdash;" that starts each caption.
Usage: python3 linkrefs.py manifest.json file.html [file.html ...]   (edits in place, prints a report)
"""
import json, re, sys
from collections import Counter

NB = r"(?:&nbsp;| )"
EXCL = {"a", "svg", "script", "style", "code", "pre", "h1", "h2", "h3", "h4", "title"}

def add_ids(s, fig_ids):
    def h3(m):
        attrs, text = m.group(1), m.group(2)
        if "id=" in attrs: return m.group(0)
        n = re.match(r"(\d+)\.(\d+)\b", text)
        return f'<h3 id="s{n.group(1)}-{n.group(2)}"{attrs}>{text}</h3>' if n else m.group(0)
    s = re.sub(r"<h3([^>]*)>(.*?)</h3>", h3, s, flags=re.S)
    it = iter(fig_ids)
    s = re.sub(r"<figure>", lambda m: f'<figure id="{next(it)}">', s)
    return s

def build_targets(s):
    ids = set(re.findall(r'\sid="([^"]+)"', s))
    figs = {}
    for m in re.finditer(r'<figure id="([^"]+)">.*?<figcaption><b>Figure (\d+) &mdash;', s, re.S):
        figs[m.group(2)] = m.group(1)
    exs = {}
    for m in re.finditer(r'<div class="ex[^"]*" id="([^"]+)"[^>]*>\s*<div class="ex-head"><span class="ex-t">Example (\d+\.\d+)', s):
        exs[m.group(2)] = m.group(1)
    return ids, figs, exs

def link(href, text, ids, stats, kind):
    if href.lstrip("#") not in ids:
        stats["unresolved " + kind] += 1
        return None
    stats[kind] += 1
    return f'<a href="{href}">{text}</a>'

def process_text(t, ids, figs, exs, stats):
    def sec_href(a, b):
        return f"#s{a}-{b}" if b else f"#s{a}"
    def r_sect(m):
        a, b = m.group(2), m.group(3)
        out = link(sec_href(a, b), m.group(0), ids, stats, "section")
        return out or m.group(0)
    t = re.sub(r"(&sect;)(\d+)(?:\.(\d+)(?!\d))?", r_sect, t)
    t = re.sub(rf"\b([Ss]ection{NB})(\d+)(?:\.(\d+)(?!\d))?", r_sect, t)
    def r_lessons(m):
        head, nums = m.group(1), m.group(2)
        def one(n):
            return link(f"#s{n.group(0)}", n.group(0), ids, stats, "lesson") or n.group(0)
        return head + re.sub(r"\d+", one, nums)
    t = re.sub(rf"\b(Lessons{NB})(\d+(?:(?:, | and |&ndash;|-)\d+)*)", r_lessons, t)
    def r_lesson(m):
        return link(f"#s{m.group(2)}", m.group(0), ids, stats, "lesson") or m.group(0)
    t = re.sub(rf"\b(Lesson{NB})(\d+)\b(?!\.\d)", r_lesson, t)
    def r_figs(m):
        head, nums = m.group(1), m.group(2)
        def one(n):
            f = figs.get(n.group(0))
            return (link(f"#{f}", n.group(0), ids, stats, "figure") if f else None) or n.group(0)
        return head + re.sub(r"\d+", one, nums)
    t = re.sub(rf"\b(Figures{NB})(\d+(?:(?:, | and |&ndash;)\d+)*)", r_figs, t)
    def r_fig(m):
        f = figs.get(m.group(2))
        if not f:
            stats["unresolved figure"] += 1; return m.group(0)
        return link(f"#{f}", m.group(0), ids, stats, "figure") or m.group(0)
    t = re.sub(rf"\b(Figure{NB})(\d+)\b(?! &mdash;)", r_fig, t)
    def r_ex(m):
        e = exs.get(m.group(2))
        if not e:
            stats["example without target (left as text)"] += 1; return m.group(0)
        return link(f"#{e}", m.group(0), ids, stats, "example") or m.group(0)
    t = re.sub(rf"\b(Example{NB})(\d+\.\d+)(?!\d)", r_ex, t)
    return t

TAG = re.compile(r"<(/?)([a-zA-Z][a-zA-Z0-9]*)([^>]*)>|<!--.*?-->", re.S)

def process(s, ids, figs, exs, stats):
    out, pos, stack = [], 0, []
    for m in TAG.finditer(s):
        text = s[pos:m.start()]
        out.append(text if stack else process_text(text, ids, figs, exs, stats))
        out.append(m.group(0)); pos = m.end()
        if m.group(0).startswith("<!--"): continue
        close, name, attrs = m.group(1), (m.group(2) or "").lower(), m.group(3) or ""
        if attrs.rstrip().endswith("/"): continue
        excl = name in EXCL or (name == "span" and 'class="msg"' in attrs)
        if not close:
            if stack:
                if name == stack[-1][0]: stack[-1][1] += 1
            elif excl:
                stack.append([name, 1])
        elif stack and name == stack[-1][0]:
            stack[-1][1] -= 1
            if stack[-1][1] == 0: stack.pop()
    tail = s[pos:]
    out.append(tail if stack else process_text(tail, ids, figs, exs, stats))
    return "".join(out)

def main():
    man = json.load(open(sys.argv[1], encoding="utf-8"))
    fig_ids = [f["id"] for f in man["figures"]]
    for fn in sys.argv[2:]:
        s = open(fn, encoding="utf-8").read()
        if re.search(r'<figure id=', s):
            sys.exit(f"{fn}: already processed")
        s = add_ids(s, fig_ids)
        ids, figs, exs = build_targets(s)
        dup = [k for k, v in Counter(re.findall(r'\sid="([^"]+)"', s)).items() if v > 1]
        stats = Counter()
        s = process(s, ids, figs, exs, stats)
        bad = [h for h in re.findall(r'href="#([^"]+)"', s) if h and h not in ids]
        open(fn, "w", encoding="utf-8", newline="").write(s)
        print(fn, dict(stats), "| duplicate ids:", dup, "| unresolved hrefs:", sorted(set(bad)))

if __name__ == "__main__":
    main()
