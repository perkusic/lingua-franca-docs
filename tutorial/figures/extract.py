#!/usr/bin/env python3
"""One-time extraction of the inline SVG figures of the two Lingua Franca HTMLs (step 1 of the modularization).

Writes, next to this script:
  svg/<id>.svg            the exact bytes of each <svg ...>...</svg> block (no trailing newline added)
  skeleton/<name>.skel.html  each HTML with every SVG block replaced by the placeholder <!--@svg:<id>@-->
  manifest.json           ids, current figure numbers, SHA-256 of every SVG and of both HTMLs
The HTMLs themselves are not modified and remain canonical.

Usage: python3 extract.py ../lingua_franca_tutorial.html ../lingua_franca_rti.html
"""
import hashlib, json, os, re, sys

HERE = os.path.dirname(os.path.abspath(__file__))
SVG_RE = re.compile(rb"<svg\b.*?</svg>", re.S)
CAP_RE = re.compile(rb"<figcaption><b>Figure (\d+) &mdash; (.*?)\.</b>", re.S)
STOP = {"a", "an", "the", "of", "on", "in", "as", "and", "for", "to", "at", "is", "its", "one", "two", "with", "by", "that", "why", "how", "what", "when", "which", "where"}

def sha(b): return hashlib.sha256(b).hexdigest()

def slug(title):
    t = re.sub(rb"<[^>]+>", b"", title).decode("utf-8")
    t = re.sub(r"(&rsquo;|')s\b", "", t)
    t = re.sub(r"&[a-z]+;|&#\d+;", " ", t).lower()
    words = [w for w in re.findall(r"[a-z0-9]+", t) if w not in STOP]
    return "fig-" + "-".join(words[:5])

def figures(html):
    """[(start, end, svg_bytes, number, title)] in document order; each SVG is followed by its caption."""
    out = []
    for m in SVG_RE.finditer(html):
        c = CAP_RE.search(html, m.end())
        out.append((m.start(), m.end(), m.group(0), int(c.group(1)), c.group(2)))
    return out

def main(paths):
    docs = {os.path.basename(p): open(p, "rb").read() for p in paths}
    names = list(docs)
    figs = {n: figures(docs[n]) for n in names}
    counts = {n: len(f) for n, f in figs.items()}
    assert len(set(counts.values())) == 1, counts
    for n, b in docs.items():
        assert b"<!--@svg:" not in b, f"placeholder already present in {n}"
    ref = figs[names[-1]]  # ids from the reference edition's caption titles
    ids, seen = [], set()
    for k, f in enumerate(ref):
        i = slug(f[4])
        if i in seen:
            i = f"{i}-{k+1}"
        seen.add(i); ids.append(i)
    os.makedirs(os.path.join(HERE, "svg"), exist_ok=True)
    os.makedirs(os.path.join(HERE, "skeleton"), exist_ok=True)
    manifest = {"note": "HTMLs remain canonical; this is a verified extraction, not the source of truth.",
                "placeholder": "<!--@svg:{id}@-->", "documents": {}, "figures": []}
    for k, i in enumerate(ids):
        variants = {n: figs[n][k][2] for n in names}
        distinct = {sha(v) for v in variants.values()}
        entry = {"id": i, "numbers": {n: figs[n][k][3] for n in names}, "title": re.sub(rb"<[^>]+>", b"", ref[k][4]).decode()}
        if len(distinct) == 1:
            v = variants[names[0]]
            fn = f"svg/{i}.svg"
            open(os.path.join(HERE, fn), "wb").write(v)
            entry.update(file=fn, sha256=sha(v), bytes=len(v), shared=True)
        else:  # keep each edition's bytes separately
            entry["shared"] = False
            entry["variants"] = {}
            for n, v in variants.items():
                fn = f"svg/{i}.{n.split('.')[0]}.svg"
                open(os.path.join(HERE, fn), "wb").write(v)
                entry["variants"][n] = {"file": fn, "sha256": sha(v), "bytes": len(v)}
        manifest["figures"].append(entry)
    for n in names:
        b, parts, pos = docs[n], [], 0
        for k, f in enumerate(figs[n]):
            parts += [b[pos:f[0]], f"<!--@svg:{ids[k]}@-->".encode()]
            pos = f[1]
        parts.append(b[pos:])
        skel = b"".join(parts)
        sk = f"skeleton/{n.rsplit('.', 1)[0]}.skel.html"
        open(os.path.join(HERE, sk), "wb").write(skel)
        manifest["documents"][n] = {"sha256": sha(b), "bytes": len(b), "skeleton": sk, "skeleton_sha256": sha(skel),
                                    "figures": [ids[k] for k in range(len(ids))]}
    json.dump(manifest, open(os.path.join(HERE, "manifest.json"), "w"), indent=1, ensure_ascii=False)
    print(f"{len(ids)} figures, {sum(1 for e in manifest['figures'] if e['shared'])} shared")

if __name__ == "__main__":
    main(sys.argv[1:])
