# Extraction of the SVG figures — step 1 (15 October 2026)

**Status: verified extraction. The two HTMLs in `tutorial/` remain canonical.** Nothing in this folder is a source of truth yet; editing an `.svg` here changes nothing in the documents. Text, styles, scripts, quiz and references were not touched.

## Baseline and result

| document | bytes | SHA-256 before extraction | SHA-256 rebuilt from skeleton + svg/ | result |
|---|---|---|---|---|
| `lingua_franca_tutorial.html` | 795116 | `14dc3261ab2a71df940af50775e27a046e68ebf26aad9c22ec9646495bff31d8` | `14dc3261ab2a71df940af50775e27a046e68ebf26aad9c22ec9646495bff31d8` | identical byte for byte (`cmp` and SHA-256) |
| `lingua_franca_rti.html` | 612173 | `df4129fb2d9dad10956efb85a4553a41bec038f2d9fe858759740e6175716b69` | `df4129fb2d9dad10956efb85a4553a41bec038f2d9fe858759740e6175716b69` | identical byte for byte (`cmp` and SHA-256) |

The rebuild was run in the cloud workspace and again with `python3` 3.10 on this computer, against the files in this folder: PASS in both, and `cmp` of the rebuilt files against the canonical HTMLs reports no difference.

**Incident during delivery (keep in mind for every later step).** The first copy of `svg/` to this computer failed the check: the file-delivery channel had inserted C2PA provenance metadata (`xmlns:c2pa=…`) into every `.svg` (for example 4 023 → 11 797 bytes), while the skeletons, the manifest and the scripts arrived byte-identical. The check detected it on all 43 files. The SVGs in this folder were therefore regenerated **on this computer** by running `extract.py` on the canonical HTMLs; the resulting `manifest.json` is byte-identical to the one produced in the cloud, so every SVG hash matches the baseline. Rule: never move `.svg` files through a channel that may rewrite images; regenerate them locally, or move them inside a container (archive, JSON) and verify the hashes after unpacking.

**Re-extraction, 15 October 2026, 07:20.** After two text edits in the canonical HTMLs (a link to Example&nbsp;16.1 and to section&nbsp;16.3 in the tutorial; `id="s16-3"` on the h3 of 16.3 in both editions), `extract.py` was run again on this computer: the 43 SVG hashes are unchanged, the skeletons and document hashes above are the new ones, and `assemble.py --check` is PASS. First extraction's baseline: tutorial `88973f23033e663e`, reference `478dcccefacef892`.

**Re-extraction, 15 October 2026, 08:40.** After cross-references became links (`lf-docs/linkrefs.py`) and every `<figure>` received its stable id as `id` attribute, `extract.py` was run again on this computer: the 43 SVG hashes are unchanged, `assemble.py --check` is PASS.

**15 October 2026, afternoon: 44 figures.** A new Figure 1 (`fig-actor-model-message`, the actor model with an animation that runs with Play) was added in §1.1; the former Figures 1–43 are now 2–44. Captions, links and one figure number written inside an SVG were renumbered from the stable ids. `fig-clock-synchronization-round.svg` changed for that reason only ("run of Figure 41"). The figure-level findings below date from the 43-figure extraction.

**16 October 2026: no section signs.** Cross-references written with a section sign became words in both editions (`lf-docs/sectwords.py`): "section&nbsp;N.M" for a subsection, "Lesson&nbsp;N" (tutorial) or "section&nbsp;N" (reference) for a top-level section, with homogeneous lists as "sections&nbsp;4.10 and 4.12". Eight SVG texts changed with them, in seven files. One of them, `fig-tag-nothing-send-under-both`, now reads "Lesson 4" in the tutorial and "section 4" in the reference, so it is the first figure stored per edition (`svg/fig-tag-nothing-send-under-both.lingua_franca_tutorial.svg` and `….lingua_franca_rti.svg`): 44 figures, 43 shared. `assemble.py --check` is PASS.

**16 October 2026, morning: numbered tables.** Every `<table>` received a stable `id` and a caption "Table&nbsp;N &mdash; title." (`lf-docs/tablecaps.py`); vague mentions ("the table") became links to "Table&nbsp;N". One SVG text changed with it (`fig-closure-successive-approximations`: "Table 3 of section 4.13"). Tables are not extracted: they stay in the skeletons. `assemble.py --check` is PASS. Later the same morning, "section" became "Section" in every numbered reference (eight SVG texts among them); PASS again.

## Findings

- **43 SVG blocks per edition, all inside `<figure>`; all 43 are byte-identical between the two editions.** Each is therefore stored once (`svg/<id>.svg`, 43 files, 286 704 bytes in total).
- **The four figure blocks that differ between editions (Figures 29, 34, 35 and 41) differ only in their captions**, never in the SVG: Figure 29 says "Example&nbsp;4.1" in the tutorial and "&sect;4.13" in the reference; Figures 34, 35 and 41 say "Lesson&nbsp;3" / "Lesson&nbsp;3" / "Lesson&nbsp;4" against "&sect;3" / "&sect;3" / "&sect;4" (numbers of the 43-figure extraction; since 16 October 2026 the reference writes "section" instead of the sign). Captions were not extracted: they stay in each edition's skeleton unchanged, so the four variants are preserved as they are. The extractor would have kept separate per-edition SVG files had any SVG differed; none did.
- The placeholder `<!--@svg:<id>@-->` did not occur in either document before extraction.

## Identity and numbering

**Since 15 October 2026 (afternoon), identity is read from the HTML:** every figure is written `<figure id="<stable id>">`, and `extract.py` matches the editions by that id, not by position or caption. A figure may sit in a different place in each edition, or exist in only one (recorded under `editions` in the manifest). The extractor stops with a named error if a `<figure>` lacks an id, contains other than one `<svg>`, lacks a caption, or reuses an id. `assemble.py` checks each skeleton's placeholders before rebuilding and names unknown ids, duplicated placeholders, figures without a placeholder, missing SVG files and order changes. Rerunning the new extractor on the current HTMLs produced a byte-identical `manifest.json`, the same 43 SVGs and the same skeletons.

The paragraph below records how the ids were first chosen.


Each figure has a stable identifier derived once from the reference edition's caption title (`fig-` plus up to five significant words). The identifier is the file name and the placeholder; it carries no number. Today's numbers are recorded in `manifest.json` only for cross-checking. The numbers printed in captions and body text are still literal text in the skeletons: assigning them at assembly is a later step, not done here.

## Files

| path | content |
|---|---|
| `svg/<id>.svg` | exact bytes of one `<svg …>…</svg>` block, **no trailing newline** (an editor that adds one makes the check fail, by design) |
| `skeleton/*.skel.html` | each HTML with every SVG block replaced by its placeholder |
| `manifest.json` | ids, numbers per edition, SHA-256 and size of every SVG, of both skeletons and of both original HTMLs |
| `extract.py` | the one-time extraction (kept for the record; rerunning it overwrites this folder) |
| `assemble.py` | rebuild and check |

## How to check

    cd tutorial/figures
    python3 assemble.py --check              # PASS / FAIL, compares with the manifest and the canonical HTMLs
    python3 assemble.py --check --out /tmp/x # also writes the rebuilt files; refuses to overwrite the canonical ones

Negative tests run: one byte appended to an SVG file → both editions MISMATCH and the file is named; one word changed in a skeleton → that edition MISMATCH, the other still IDENTICAL.

**Once a canonical HTML is edited, the extraction is stale** (`--check` reports "canonical file DIFFERS") until `extract.py` is run again. That is expected while the HTMLs remain canonical.

## Inventory of figures

| # (both editions) | id | bytes | SHA-256 (first 16) |
|---|---|---|---|
| 1 | `fig-actor-model-message` | 14193 | `747d9b7b9903a539` |
| 2 | `fig-message-time-not-order` | 7801 | `e3474cc43e8199a9` |
| 3 | `fig-reactor-ports-levels` | 3332 | `8551906273c15e9e` |
| 4 | `fig-federation-architecture` | 2892 | `c5706dc1353ef76a` |
| 5 | `fig-joining-federation` | 7595 | `8613a52fae69911d` |
| 6 | `fig-question-rti-answers` | 5007 | `c3306a8cda16676e` |
| 7 | `fig-grant-decision` | 3078 | `19a19870b6fd21c2` |
| 8 | `fig-safety-conditions-wire` | 4740 | `d443ca6bbd526af4` |
| 9 | `fig-zero-delay-cycle-message-message` | 5452 | `3d2bf89b0fedf346` |
| 10 | `fig-superdense-time` | 4023 | `d4b4162605c40799` |
| 11 | `fig-graph-becomes-decision` | 11654 | `97c9585b56094ca7` |
| 12 | `fig-rti-cannot-trust-net-alone` | 9061 | `efd08eaef2f97f4d` |
| 13 | `fig-cost-net` | 2617 | `99605771a563c1be` |
| 14 | `fig-example-federation` | 3072 | `d3f073e5b1ecd93c` |
| 15 | `fig-releasing-cycle` | 2810 | `1d3682f665138325` |
| 16 | `fig-dnet-provides` | 2477 | `dc35a9a1c7c828a0` |
| 17 | `fig-algebra-comes-from` | 6986 | `d5dd70c973009831` |
| 18 | `fig-absence-safe-assume` | 8068 | `2cab26ed6efd1ac9` |
| 19 | `fig-ways-establish-tag-safe` | 8201 | `9eefe7dae87eb2e1` |
| 20 | `fig-cycles-can-exist` | 9356 | `7793790cda63e3af` |
| 21 | `fig-federate-scheduler` | 2834 | `b847b71c52c40c0a` |
| 22 | `fig-reaction-module` | 3715 | `e0b1677f18aef600` |
| 23 | `fig-codesign-top-level` | 4439 | `0c60fbd65c16a453` |
| 24 | `fig-codesign-build` | 3925 | `bdf5cb5d8cb08984` |
| 25 | `fig-cascade-snapshot` | 6944 | `235968c0ae76a450` |
| 26 | `fig-three-quantities-timeline` | 9785 | `5fb2d45b735a358d` |
| 27 | `fig-interpret-time-based-rendezvous` | 3760 | `11d952a1b91b44b5` |
| 28 | `fig-four-orders` | 11631 | `13f7c4ae6fd1603c` |
| 29 | `fig-fixed-points-monotone-function` | 11255 | `94ec1cb1b98be845` |
| 30 | `fig-dependency-graphs-program` | 7340 | `5783a950f1d912fb` |
| 31 | `fig-closure-successive-approximations` | 10526 | `d7ff89e5ef572dab` |
| 32 | `fig-any-relaxation-order-ends-least` | 8079 | `4dff624511b6068a` |
| 33 | `fig-fixed-point-tag` | 7527 | `579fb525620c17e6` |
| 34 | `fig-acyclic-reaction-graph-gives-exactly` | 8986 | `47b8a59d7a75f4b0` |
| 35 | `fig-compiler-builds-example-16-1` | 10482 | `4fcc8cd8e6e590f2` |
| 36 | `fig-startup-between-neighbors-logged` | 7027 | `f2d67c100a4a0672` |
| 37 | `fig-startup-coordinator-state-machine` | 6582 | `ab5b5b995068c04c` |
| 38 | `fig-tcp-network-channel-state-machine` | 5794 | `e0f67f750849063e` |
| 39 | `fig-clock-synchronization-round` | 4157 | `1a59636eb7461b6f` |
| 40 | `fig-micro-lf-federate-advances-state` | 9863 | `533f82dfce1dbf26` |
| 41 | `fig-example-16-1-dst-physical` | 10861 | `472ee71e8ce61424` |
| 42 | `fig-tag-nothing-send-under-both` | 5896 / 5897 | `b03e8a4dfd5afa97` / `da899da86a2c10d4` (tutorial / reference) |
| 43 | `fig-micro-lf-decision-datapath-interpretation` | 9087 | `66f5b03bd51f9cca` |
| 44 | `fig-micro-lf-program-meets-hardware` | 8009 | `71b1afbaf09f9c41` |

Skeleton hashes: `skeleton/lingua_franca_tutorial.skel.html` `ccb55174f171ca66`; `skeleton/lingua_franca_rti.skel.html` `3aff332a67860f74`.
