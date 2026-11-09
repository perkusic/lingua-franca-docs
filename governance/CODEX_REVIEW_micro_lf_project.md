# Codex review brief: micro-LF on FPGA project definition and validation example

Paste this file (or point Codex at it) as the task. It is written for an agent that has a shell,
can clone repositories and has network access to GitHub, Maven Central and the Gradle plugin portal.

## 1. Role and goal

You are an independent reviewer. Another agent (Claude) wrote the material under review; you have
not seen how it was produced and should not trust its claims until you have checked them. Your job
is to find what is wrong, unclear or unverifiable, fix what you can prove, and report the rest.

Material under review, in `perkusic/lingua-franca-docs`:

| Item | Where |
|---|---|
| A. Validation example (code, build, results) | `micro-lf/validation/` — `Validate3.ulf`, `sensor.c`, `fusion.c`, `CMakeLists.txt`, `run.sh`, `README.md` |
| B. Project definition | `micro-lf/project-definition.md` (a Markdown snapshot of the Claude Doc "micro-LF on FPGA — Project Definition", revision 31; its five diagrams appear as placeholders, and their content is described in the surrounding text and tables) |
| C. Consistency sources | `hw-rti/HW-RTI_project_specification.html`, `program/implementation_guide.html`, `micro-lf/micro_lf_fpga_design_note.html`, `tutorial/lingua_franca_tutorial.html` (Lesson 16, Section 5.3) |

Ground truth for runtime behaviour: `lf-lang/reactor-uc` at commit
`df7cfeae7083509621b8f5080c066051112e2500` (short `df7cfea`, 7 September 2026). Read the code,
not documentation, when the two disagree.

## 2. Tasks, in order

### Task 1 — Build and compile the real program (highest value)

1. Clone `lf-lang/reactor-uc` at `df7cfea` with submodules.
2. Locate the micro-LF code generator (`ulfc`) that matches that commit (the `lfc` sources / the
   `uc` target in `lf-lang/lingua-franca`, or the generator referenced from reactor-uc's own
   tests and CI). Record the repository, commit hash and date you used.
3. Build it (Gradle). Compile `micro-lf/validation/Validate3.ulf` for Linux.
4. If `Validate3.ulf` does not compile, fix it with the smallest change that keeps its meaning
   (two sensors, periods 100 and 150 ms, fusion with `@maxwait`, `tardy` handler, shutdown check
   8 reactions / sum a 21 / sum b 10, timeout 550 ms). List each change and why.
5. Diff the generated C against `sensor.c` and `fusion.c`: entry-point arguments
   (`LF_ENTRY_POINT_FEDERATED`), connection constructors and their `@maxwait` argument, the
   startup-coordinator neighbour counts, clock-sync setting, channel setup and ports. Report every
   difference that could change behaviour.

### Task 2 — Reproduce the measurements

Run V1, V2, V3 and the `@maxwait` sweep (55, 59, 60, 61, 62, 65, 70 ms; three runs each), first
with the hand-written C, then with the generated code from Task 1.

Expected values (measured on Linux 6.18 x86-64, 2 cores, GCC 13.3, CMake 3.28, TCP loopback):

| Run | Expected |
|---|---|
| V1 (W = 30 ms) | 8 reactions, 0 tardy, PASS; lags ≈ 0.4 ms at tags 0 and 300, ≈ 30.1–30.3 ms at 100, 150, 200, 400, 450, 500 |
| V2 (W = 30 ms, B 60 ms late at tag 300) | 8 reactions, 1 tardy at (300 ms, 1), PASS; tag 300 with b absent at ≈ 30.1 ms |
| V3 (W = 100 ms, B late) | 8 reactions, 0 tardy, PASS; tag 300 with a and b at ≈ 60.6 ms; ≈ 50.3–50.6 ms at 100, 150, 400, 450; ≈ 100.3 ms at 200; ≈ 51 ms at 500 |
| Sweep | W ≤ 60 ms → 1 tardy each run; W ≥ 61 ms → 0 tardy, lag at tag 300 ≈ 60.6–60.9 ms |

Logical results (presence per tag, tardy count, sums) must match exactly. Lags may differ by host;
report yours with the host description. Explain any logical mismatch from the reactor-uc source.

Open point to settle: in V3 the decision for tag 500 lands at ≈ 551 ms, after the sensors' 550 ms
timeout. Find which event resolves it (a message, the stop protocol, or the deadline) and state it
with the code path.

### Task 3 — Check the technical claims

Verify each claim against reactor-uc `df7cfea` and cite file and line (permalink with the full hash):

1. A federate proceeds at tag g when every input has `last_known_tag ≥ g`; otherwise it waits
   until physical time g.t + W, where W is the largest `@maxwait` among unresolved inputs.
2. KNOWN and EXPIRED are distinct outcomes.
3. A message whose tag is already past is delivered to the `tardy` handler at (t, m+1) with its
   value, and is not lost.
4. A later-tagged message on a connection makes earlier tags on that connection known (the source
   of the ≈ 50 ms lags in V3).
5. Payload bound 832 bytes; Platform interface = five functions plus a mutex
   (`get_physical_time`, `wait_until`, `wait_for`, `wait_until_interruptible`, `notify`).
6. The three `reactor-uc` behaviours listed under DEC-14 / the risks table (knowledge advancing
   after a dropped event, payload release on failure paths, the joining-time write) exist as
   described.

### Task 4 — Review the project definition (item B)

Check for:
- internal consistency: every MR-xx names a test T-x that exists and actually exercises it;
  every phase exit criterion is checkable; deliverables D-1…D-7 map to phases; gates G1–G3
  are defined before they are used;
- consistency with item C: OQ-7, R-9, KPI-1/2, M-4, DEC-09…DEC-14, stages B1–B4 say what the
  project definition says they say (open the anchors `#tab-open-questions`, `#tab-risks`,
  `#tab-kpis`, `#tab-milestones`, `#dec-09`…`#dec-14`, `#tab-stages`);
- feasibility of the schedule estimate (P0–P2 ≈ 11 weeks) and of the ZCU106 resource figures
  (XCZU7EV: 230,400 LUTs, 460,800 FFs, 312 BRAM, 96 URAM) — check the AMD data sheet;
- claims stated without evidence, and any number that does not trace to a measurement or source.

## 3. House rules (the repository CI enforces most of them)

Apply these to every text you change in the HTML documents and to any text you propose for B:

- No "§". Write "Section N.M" (capital S, also in plurals: "Sections 4.10, 4.12"); in the tutorial
  top level, "Lesson N".
- Tables are numbered and captioned "Table N — Title." with an id `tab-…`; refer to them as
  "Table N" with a link, never "the table".
- Figures "show"; never "draw", "draws", "drawn", "drew".
- No positional references ("the figure above", "below", "described above", "the next figure");
  use "Figure N" with a link.
- Captions: title plus at most ~300 characters; explanation goes in a "How to read Figure N."
  paragraph and the conclusion in "What Figure N shows."
- Code citations: full path, repository, host, commit (full hash) and date, with a
  commit-pinned permalink.
- Run `python3 .github/scripts/check_html.py` (see `.github/workflows/check.yml` for the
  arguments per file) before committing; it must pass.

## 4. Constraints

- Do not change a measured number unless you re-measured it; if you did, give the new value, the
  host and the number of runs.
- Do not edit `reactor-uc`; behaviours you think are defects go into the report as candidate
  upstream issues.
- Keep changes small and in separate commits by task. Do not reformat files you are not fixing.
- Do not touch `governance/HANDOFF_*.md`, `governance/INDEX.md` or the `INVENTARIO`/report
  files; the owner updates them.
- Commit identity: `perkusic <perkusic@dee.ufcg.edu.br>`. Work on a branch
  `codex/review-micro-lf` and open a pull request; do not push to `main`.

## 5. Deliverable

A file `micro-lf/validation/REVIEW_codex.md`, committed on the branch, with:

1. Environment: host, OS, compiler, CMake, Java/Gradle, `ulfc` source repository + commit.
2. Task 1 result: compiled yes/no, changes to `Validate3.ulf`, diff summary generated vs
   hand-written.
3. Task 2 result: a table of your measured values next to the expected ones; PASS/FAIL per run;
   the explanation for tag 500 in V3.
4. Task 3 result: each claim → CONFIRMED / WRONG / PARTLY, with permalink evidence.
5. Task 4 result: findings for B, each with severity (high / medium / low), the exact passage
   quoted, why it is a problem, and the proposed replacement text.
6. A short list of what you could not verify and why.

Severity guide: **high** = a wrong fact, a wrong number or a test that cannot pass; **medium** =
an unverifiable claim, an inconsistency between documents, a missing link between requirement and
test; **low** = wording, style rules, clarity.
