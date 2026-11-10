# Codex review brief: LF hardware coordination project definition (ZCU106)

Paste this file (or point Codex at it) as the task. It is written for an agent with a shell, `git`,
network access to GitHub, and `mmdc` (Mermaid CLI) or another way to render Mermaid.

## 1 Role and goal

You are an independent reviewer. Another agent (Claude) wrote the material under review from a design
note and a task prompt. Do not trust its claims until you have checked them. Find what is wrong,
missing, unclear or unverifiable; fix what you can prove; report the rest with evidence.

### Material under review (repository `perkusic/lingua-franca-docs`, branch `main`)

| Item | Path |
|---|---|
| **R1. Project definition** (main object of the review) | `program/lf_hardware_coordination_project_definition.md` |
| **R2. Proposed changes to other documents** | `program/lf_hardware_coordination_proposed_changes.md` |

### Inputs the review is judged against

| Item | Path | Role |
|---|---|---|
| Task prompt | `program/sources/lf_hardware_coordination_task_prompt.md` | **The requirements R1 must satisfy.** |
| Design note (original) | `program/sources/lf_hardware_coordination_zcu106_expanded.docx` | Technical basis, "a design proposal". Must stay unchanged. |
| Design note (Markdown conversion, with figures) | `program/sources/lf_hardware_coordination_zcu106_expanded.md`, `program/sources/lf_hardware_coordination_zcu106_media/` | Same content, readable as text. If the two disagree, the `.docx` wins. |
| HW-RTI specification, rev. 0.2 | `hw-rti/HW-RTI_project_specification.html` | Source of every [SPEC] claim about the HW-RTI |
| Implementation guide, rev. 1 | `program/implementation_guide.html` | DEC-09 to DEC-14, stages |
| micro-LF FPGA design note, rev. 1 | `micro-lf/micro_lf_fpga_design_note.html` | Track B background |
| Earlier review (Linux evidence) | `micro-lf/validation/REVIEW_codex.md` on branch `codex/review-micro-lf`, commit `f429e437f5d6db66269ec88ef4f09a73e3458adf` | Source of every [LNX] claim |

### Source code at the pinned commits (clone them; do not use other revisions)

| Repository | Commit |
|---|---|
| `erlingrj/reactor-chisel` | `d94dd2d272ce46145ac13c3c6a141bc2ba9dfca1` and `6c8aecd0211acc4c1e58d1cdd9ae950880a25204` |
| `erlingrj/lingua-franca` | `f60c83c5fbf0f0c26c780c28e58e50c0fc60a608` |
| `lf-lang/reactor-uc` | `df7cfeae7083509621b8f5080c066051112e2500` |
| `lf-lang/reactor-c` | `ccd05124e4b2086de045a6857702ae5636da30a6` |

## 2 Tasks, in order

### Task 1 — Compliance with the task prompt

Go through the task prompt sentence by sentence and record, for each requirement, whether R1 meets it
(MET / PARTLY / MISSING), where (section of R1), and what is missing. At least these:

1. Standalone, in English, usable by an engineer to plan, implement and validate.
2. The four kinds of evidence kept separate: verified in the original Codesign source; provided by the
   existing ZCU106 port; demonstrated by the Linux experiment; still to build or test on the board.
   Uncertainty stated where a repository, revision or board artifact was unavailable.
3. No invented commands, interfaces, measurements or test results. Flag every command, interface
   name, register, signal or number in R1 that has no source and is not labelled as proposed or
   estimated.
4. Track A and Track B kept distinct, with the content the prompt lists for each (two independently
   controlled hardware participants, static acyclic topology, final TAG grants, the Codesign
   `TAG`/`NET`/`LTC`/`WRITE`/`READ` interface as starting point, the four semantics to adapt; for
   Track B the A53 TCP baseline, then fabric channel and shared time base, then a later hardware
   participant with explicit input knowledge before finite waits and tardiness).
5. The recommendation: Track A as first hardware federation milestone, **conditional** on a verified
   grant policy for the chosen topology; Track B Linux/A53 work independent; evidence that would justify
   moving to decentralized advancement; the statement that peer-to-peer transport alone does not make
   coordination decentralized.
6. Deliverables 1–6 of the prompt, including: block diagrams for both tracks; a flowchart for **each**
   advancement policy; one sequence diagram from sender execution through receiver admission and
   completion; owner of each decision and buffer; the reuse map for the four sources with the three
   categories; each requirement mapped to a test; milestones and gates defined before use; the
   first-milestone trace; validation covering delayed data, explicit absence, microsteps,
   backpressure, reset and tardiness; semantic checks separate from performance; OQ-7 and what else
   it needs; risks, open decisions with the evidence that closes each, first tasks; estimates kept
   apart from measurements.
7. The review findings the prompt names, worded as the prompt requires: 61 ms as the smallest
   successful **tested** wait on that Linux host; the TCP disconnect releasing tag 500 in the
   hand-written V3 run; generated C differing in queue capacities, keep-alive and TCP setup. No Linux
   result presented as a ZCU106 measurement or as a worst-case guarantee.
8. Citations with repository, full commit, file path and pinned link where available.
9. The design note and canonical sources preserved; proposed changes listed separately (R2).

### Task 2 — Verify every [SRC] claim

For each statement labelled [SRC] in R1 (Section 4.1 table, the [SRC] row of Section 4.3, Section 6.2,
Section 8.2, Section 9) and in R2, open the cited file at the cited commit and line range and record
CONFIRMED / WRONG / PARTLY, with the exact lines. Check in particular:

- `NEVER` comparing above `FOREVER` in `Tag.scala`, and whether any reactor-chisel comparison actually
  reaches the sentinels in the paths that Track A would use.
- The 32-bit truncation at `CodesignFpgaWrapperTransformation.java` L374 and the claimed defect in
  the next-event reconstruction at L423–L424 (`nextEventTag + (now.count() >> 32)`). Decide whether
  it is a defect, or whether another part of the generated code makes it correct. If it is a defect,
  write the minimal reproducer (inputs and expected versus actual value).
- `6c8aecd` being an ancestor of `d94dd2d`, with only `LICENSE` and `README.md` changed between them.
- The `LtcHandler` completion rule (all outputs fired; with no outputs, an input port ready again) and
  the `reactionDone` default.
- `SwCommand` forwarding only on a change of value.
- The reactor-uc fields in `federated.h` L114–L118, `intended_tag` in `event.h`, the five platform
  functions in `platform.h`, the S4NOC channel for Patmos, and the `max_wait` acquisition rule.
- Every pinned link in R1 and R2 opens the cited lines (28 GitHub links at the time of writing).

### Task 3 — Verify every [SPEC] and [LNX] claim

- [SPEC] against the HTML specifications: the HW-RTI status line ("Proposed. No implementation
  exists"), non-goal NG-6 and `MSG_SENT`, FR-20 (a)–(c), the HW-RTI M-2 exit criterion, OQ-7 being
  "not a measurement task", OQ-8, configuration C-C, DEC-09, DEC-10, DEC-11.
- [LNX] against `REVIEW_codex.md` at `f429e43`: hosts, number of runs, the sweep values, the tag-500
  explanation, the generated-versus-hand-written differences, and the reactor-uc behaviours quoted.
- Any [PORT] statement must not claim more than "reported to work"; flag any that does.

### Task 4 — Technical soundness (most important after Task 1)

Check the design, not only the citations. Known weak points to examine, at least:

1. **Figure 5 grant justification.** The sequence grants `TAG_B = min(h0, h)` after the frame's
   admission. Under the reactor-c algorithm (`rti_common.c` at `ccd0512`), a final grant to B also
   depends on what A can still send (A's next event or completion plus the connection delay), not only
   on admission of the frames already sent. Determine whether the sequence omits a necessary step (for
   example A's NET after its LTC) and whether, as drawn, the grant could be premature. Propose the
   corrected sequence.
2. **In-transit accounting.** R1 clears the in-transit entry on admission at the receiver. Check how
   reactor-c records and releases in-transit messages, and whether "release on admission" is
   equivalent, weaker or stronger. State the input-closure condition that the reference coordinator
   actually needs.
3. **`min(TAG, NET)` at the adapter.** Show that presenting `min(TAG, NET)` to the unchanged
   scheduler never lets it execute beyond the granted tag and never blocks it forever (for example
   when NET changes after a grant is retained).
4. **Completion rule.** Is "LTC after every output frame is accepted by the data path" sufficient for
   downstream grants, or must it wait for admission at the receiver? Is the no-output case handled?
5. **Microsteps.** Track A rejects non-zero microsteps; Track B's tardy path in reactor-uc retries at
   microstep + 1. Check whether B-HW2 on a reactor-chisel participant (no microsteps) can implement the
   tardy path as R1 describes, and what must change.
6. **Decentralized flowchart (Figure 4).** Compare with the reactor-uc acquisition logic: candidate
   selection, the wait for physical time, the per-input check, the deadline, and the snapshot
   re-check. Note where Figure 4 is stricter than the runtime (intentionally) and where it is wrong.
7. **Timing budget (Section 8.4).** Check that *D_i(g)* ≥ *B_i(g)* is stated as a verification
   condition and is not confused with the runtime's `max_wait` rule.
8. **Reference coordinator on the A53 (AS-4, OD-2).** Is running `rti_common.c` standalone as the
   coordinator of hardware participants feasible at `ccd0512` (globals, socket coupling, threading)?
   What is the smallest harness? The HW-RTI plan already intends a standalone build with a trace shim
   at its M-0; say whether R1 relies on more than that.
9. **Two hardware participants (AS-3).** From the Codesign compiler and reactor-chisel, determine
   whether two top-level blocks with separate `CoordinationIO` can coexist in one design, and what in
   the platform wrapper would need to change.
10. **Gates and milestones.** Every gate is defined before first use; every exit criterion is
    checkable; no milestone depends on a later one; the first-milestone acceptance trace (Section 10.3)
    is sufficient to show final grants, correct presence/absence and no premature completion.

### Task 5 — Review R2

For each entry DN-1 … RP-2: is the problem real, is the evidence correct, is the proposed text
accurate and minimal? Add entries R2 is missing (for example, changes your Task 4 findings imply for
the HW-RTI specification).

### Task 6 — Form

- All Mermaid diagrams render (`mmdc`), and each diagram matches its caption and the text.
- Internal anchors (`#…`) resolve; table and section references point to the right place.
- House rules: no "§"; "Section N" capitalized; figures "show", never "draw"; no positional references
  ("above", "below", "the next figure") used to point at content; every table and figure referred to
  by number.
- Labels: every factual sentence carries one of [SRC], [PORT], [LNX], [SPEC], [PROP], [EST], or is a
  definition, requirement or plan.

## 3 Constraints

- **Do not modify** `program/sources/*` or any canonical document (HTML specifications, guide, design
  note, tutorial, `governance/HANDOFF_*`, `governance/INDEX.md`). Proposed edits to them go into R2.
- You may edit R1 and R2 to fix what you prove. Keep each fix minimal and in separate commits by task.
  Do not reformat untouched text.
- Do not change a number labelled [LNX] unless the earlier review's evidence shows it is wrong.
- Do not present a hypothesis as a finding; mark it as such.
- Commit identity: `perkusic <perkusic@dee.ufcg.edu.br>`. Work on a branch
  `codex/review-lf-hw-coordination` from `main` and open a pull request; do not push to `main`.

## 4 Deliverable

A file `program/REVIEW_codex_lf_hw_coordination.md` on the branch, with:

1. Environment: what you cloned (commits), tools and versions.
2. Task 1: the compliance table (requirement → MET / PARTLY / MISSING → location → gap).
3. Tasks 2 and 3: each claim → CONFIRMED / WRONG / PARTLY, with the lines you read.
4. Task 4: findings with severity, the passage of R1 quoted, the argument or counterexample, and the
   proposed replacement (text or diagram).
5. Task 5: verdict per R2 entry, plus new entries.
6. Task 6: rendering, anchors, house-rule and labelling findings.
7. A list of what you could not verify and why.
8. A summary of the commits you made to R1 and R2.

Severity: **high** = a wrong fact, an unsound coordination step (a grant, closure or completion rule
that can be violated), a requirement of the task prompt missing, or a test that cannot pass;
**medium** = an unverifiable claim, an inconsistency between documents, an undefined term or gate, a
requirement not mapped to a test; **low** = wording, form, house rules.
