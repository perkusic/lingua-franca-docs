#!/usr/bin/env python3
"""Extraction of the inline SVG figures of the two Lingua Franca HTMLs (step 1 of the modularization).

Identity comes from the HTML itself: every figure is written <figure id="<stable id>"> and contains exactly one
<svg>...</svg> block. The figures of the two editions are matched by that id, not by position, so they may appear
in a different order in each edition; a figure present in only one edition is recorded as such.

Writes, next to this script:
  svg/<id>.svg               the exact bytes of each SVG block (no trailing newline added); if the two editions
                             ever differ for one id, svg/<id>.<edition>.svg per edition instead
  skeleton/<name>.skel.html  each HTML with every SVG block replaced by the placeholder <!--@svg:<id>@-->
  manifest.json              ids, current figure numbers, SHA-256 of every SVG, of both skeletons and of both HTMLs
The HTMLs themselves are not modified and remain canonical. Rerunning overwrites svg/, skeleton/ and the manifest.

Usage: python3 extract.py ../lingua_franca_tutorial.html ../lingua_franca_rti.html
       (the last file named is the reference edition, whose caption titles are recorded)
"""
import hashlib, json, os, re, sys

HERE = os.path.dirname(os.path.abspath(__file__))
FIG_RE = re.compile(rb'<figure id="([a-z0-9-]+)">(.*?)</figure>', re.S)
SVG_RE = re.compile(rb"<svg\b.*?</svg>", re.S)
CAP_RE = re.compile(rb"<figcaption><b>Figure (\d+) &mdash; (.*?)\.</b>", re.S)

def sha(b): return hashlib.sha256(b).hexdigest()

def fail(msg):
    sys.exit(f"extract.py: {msg}")

def figures(name, html):
    """{id: (start, end, svg_bytes, number, title)} in document order, with structural checks."""
    out = {}
    n_svg = len(SVG_RE.findall(html))
    for m in FIG_RE.finditer(html):
        fid, body = m.group(1).decode(), m.group(2)
        svgs = list(SVG_RE.finditer(body))
        if len(svgs) != 1:
            fail(f"{name}: figure '{fid}' contains {len(svgs)} <svg> blocks, expected 1")
        cap = CAP_RE.search(body)
        if not cap:
            fail(f"{name}: figure '{fid}' has no caption of the form <b>Figure N &mdash; title.</b>")
        if fid in out:
            fail(f"{name}: figure id '{fid}' is used twice")
        s = svgs[0]
        start = m.start(2) + s.start()
        out[fid] = (start, start + len(s.group(0)), s.group(0), int(cap.group(1)), cap.group(2))
    if len(out) != n_svg:
        fail(f"{name}: {n_svg} <svg> blocks but {len(out)} <figure id=...> blocks; every SVG must sit in a figure "
             "with a stable id (see tutorial/figures/REPORT.md)")
    return out

def main(paths):
    if len(paths) < 2:
        fail("name both HTMLs, the reference edition last")
    docs = {os.path.basename(p): open(p, "rb").read() for p in paths}
    names = list(docs)
    for n, b in docs.items():
        if b"<!--@svg:" in b:
            fail(f"placeholder already present in {n}")
    figs = {n: figures(n, docs[n]) for n in names}
    ref = names[-1]
    order = list(figs[ref]) + [i for n in names[:-1] for i in figs[n] if i not in figs[ref]]
    for n in names:
        missing = [i for i in order if i not in figs[n]]
        if missing:
            print(f"note: {n} lacks {len(missing)} figure(s) present in another edition: {missing}")
    os.makedirs(os.path.join(HERE, "svg"), exist_ok=True)
    os.makedirs(os.path.join(HERE, "skeleton"), exist_ok=True)
    manifest = {"note": "HTMLs remain canonical; this is a verified extraction, not the source of truth.",
                "placeholder": "<!--@svg:{id}@-->", "documents": {}, "figures": []}
    for i in order:
        have = [n for n in names if i in figs[n]]
        variants = {n: figs[n][i][2] for n in have}
        titled = figs[ref][i] if i in figs[ref] else figs[have[0]][i]
        entry = {"id": i, "numbers": {n: figs[n][i][3] for n in have},
                 "title": re.sub(rb"<[^>]+>", b"", titled[4]).decode()}
        if len({sha(v) for v in variants.values()}) == 1:
            v = variants[have[0]]
            fn = f"svg/{i}.svg"
            open(os.path.join(HERE, fn), "wb").write(v)
            entry.update(file=fn, sha256=sha(v), bytes=len(v), shared=True)
            if len(have) < len(names):
                entry["editions"] = have
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
        in_doc = sorted(figs[n].items(), key=lambda kv: kv[1][0])
        for i, f in in_doc:
            parts += [b[pos:f[0]], f"<!--@svg:{i}@-->".encode()]
            pos = f[1]
        parts.append(b[pos:])
        skel = b"".join(parts)
        sk = f"skeleton/{n.rsplit('.', 1)[0]}.skel.html"
        open(os.path.join(HERE, sk), "wb").write(skel)
        manifest["documents"][n] = {"sha256": sha(b), "bytes": len(b), "skeleton": sk, "skeleton_sha256": sha(skel),
                                    "figures": [i for i, _ in in_doc]}
    json.dump(manifest, open(os.path.join(HERE, "manifest.json"), "w"), indent=1, ensure_ascii=False)
    print(f"{len(order)} figures, {sum(1 for e in manifest['figures'] if e['shared'])} shared")

if __name__ == "__main__":
    main(sys.argv[1:])
