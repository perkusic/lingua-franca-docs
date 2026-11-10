# Proposed changes to source documents — not applied

Companion to [`lf_hardware_coordination_project_definition.md`](lf_hardware_coordination_project_definition.md), 9 November 2026.

No document listed here has been changed. Each entry gives the passage, the problem, the evidence and a proposed replacement, for the owner to accept or reject. Commit hashes and pinned links are those of the project definition, Section 15.

## 1 Design note — *Centralized and Decentralized Coordination for LF Hardware* (8 October 2026)

<a id="tab-1"></a>

**Table 1 — # and Passage.**

| # | Passage | Problem and evidence | Proposed change |
|---|---|---|---|
| DN-1 | Section 1: "Begin with centralized coordination using the existing project HW-RTI" | The HW-RTI specification's status is "Proposed. No implementation exists; this document defines one." There is no HW-RTI RTL to use yet. | "Begin with centralized coordination, using the reference grant algorithm (reactor-c `rti_common.c`) as coordinator only after a trace oracle and a verified A53 board binding exist. Replace it with the HW-RTI after its M-2 policy result and board-interface validation." |
| DN-2 | "Scope and evidence": "The existing board port is taken as complete." | The port's reactor-chisel and LF-fork revisions are not recorded in any project document (HW-RTI OQ-8, guide DEC-11 "open"). Treating it as complete hides the risk that the cited source lines differ on the port. | "The existing board port is reported to work; its revisions are recorded and the source findings re-checked against them before integration (HW-RTI M-0)." |
| DN-3 | Section 12, "Pin a matched combination from the working ZCU106 port rather than combining these observations as a proven build." | Partly resolved by inspection: `6c8aecd` (the compiler's submodule) is an ancestor of `d94dd2d`; the six commits between them change only `LICENSE` and `README.md`, so the Chisel sources are identical. The port's own revision is still unknown. | Add: "At source level `6c8aecd` and `d94dd2d` are identical except for `LICENSE` and `README.md`; the remaining uncertainty is the port's own revision." |
| DN-4 | Section 12: "Its NEVER and FOREVER construction and unsigned comparisons need review." | Can be stated precisely: `NEVER` is `Int.MinValue` reinterpreted as unsigned (0x8000_0000) and therefore compares above `FOREVER` (0x7FFF_FFFF) ([`Tag.scala` L6–L12](https://github.com/erlingrj/reactor-chisel/blob/d94dd2d272ce46145ac13c3c6a141bc2ba9dfca1/src/main/scala/Tag.scala#L6-L12)). | Replace with that statement and the consequence (sentinels must be mapped explicitly at the adapter). |
| DN-5 | Reference [6]: "TAG and WRITE at 408–410; LTC polling at 412; READ and NET at 421–428." | Confirmed. Two further lines matter: L374 truncates logical time to `int32_t` (about ±2.1 s in nanoseconds); L423–L424 rebuild the next-event time as `nextEventTag + (now.count() >> 32)`, adding the upper word without shifting it back (candidate defect, incorrect for nonzero upper words under the same-epoch interpretation; the pinned wrapper has no compensating scale). | Add both, citing [`CodesignFpgaWrapperTransformation.java` L374 and L423–L424](https://github.com/erlingrj/lingua-franca/blob/f60c83c5fbf0f0c26c780c28e58e50c0fc60a608/core/src/main/java/org/lflang/ast/CodesignFpgaWrapperTransformation.java#L374-L424). |
| DN-6 | References [11] and [12] point to `dev.micro-lf.org` generated API pages | Unpinned; the note itself says they are "not a pinned implementation". The same fields exist at reactor-uc `df7cfea`. | Replace with [`include/reactor-uc/federated.h` L114–L118](https://github.com/lf-lang/reactor-uc/blob/df7cfeae7083509621b8f5080c066051112e2500/include/reactor-uc/federated.h#L114-L118) and [`include/reactor-uc/event.h` L31](https://github.com/lf-lang/reactor-uc/blob/df7cfeae7083509621b8f5080c066051112e2500/include/reactor-uc/event.h#L31). |
| DN-7 | Section 4: "prefer a route whose delivery ordering is visible to the coordinator, such as a simple ordered router with explicit admission acknowledgement" | The HW-RTI specification routes no payloads (non-goal NG-6) and learns of messages through `MSG_SENT`. Admission is a visibility acknowledgement, not the reference retirement event. Section 6.3 of the specification retains the pending tag through receiver LTC. | Add: "With the HW-RTI's control-only design, keep the pending tag until receiver LTC. Use ordered receiver-visible admission as a separate barrier before releasing upstream progress or dependent grants." (Open decision OD-1 of the project definition.) |
| DN-8 | Section 11 table, micro-LF row: "Reference per-input state, max_wait handling, input knowledge and scheduler transitions" | The independent review found that at `df7cfea` KNOWN and EXPIRED are not distinct runtime results, a queue insertion failure is fatal, a late message is retried (and the retry can fail), and a TCP disconnect can release a tag. These matter for anything modelled on the runtime. | Add a sentence pointing to the review's Section 4 and stating these four behaviours. |

## 2 HW-RTI project specification (revision 0.2)

<a id="tab-2"></a>

**Table 2 — # and Passage.**

| # | Passage | Problem and evidence | Proposed change |
|---|---|---|---|
| HS-1 | R-6, "Confirmed in part on 8 October 2026 …" | Inspection of 9 November 2026 adds that the compiler's submodule `6c8aecd` and `d94dd2d` have identical Chisel sources (DN-3). | Append that finding. |
| HS-2 | FR-20 (a)–(c) | FR-20 covers tag conversion, the retained grant and PTAG, but not completion or the command handshake: `LtcHandler` reports LTC when output ports fire (or, with no outputs, when an input port is ready), `reactionDone` defaults to true, and `SwCommand` drops repeated identical commands. | Add FR-20(d): "reports LTC = g only after every reaction for g has finished and every output through g has been recorded and is visible to the destination, preserving FR-19"; and FR-20(e): "uses a command handshake that cannot lose repeated commands". Require explicit tag completion for no-output reactors. Extend UT-9 and the FR-19 ordering checks. |
| HS-3 | OQ-8 | Two facts to add for the M-0 decision: the 32-bit software truncation (DN-5) and the next-event reconstruction arithmetic defect in the pinned generated wrapper; confirm reachability and whether the board port shares it. | Append. |
| HS-4 | FR-19 and Sections 6.3, 7.1 | Admission cannot retire the reference pending-tag record, and absence requires upstream exclusion as well as visibility. | Distinguish receiver-visible admission from retirement by receiver LTC. Specify how the data/control binding orders send accounting, receiver visibility and upstream progress. Add a delayed-admission and still-possible-upstream-send case to DT-6. |
| HS-5 | FR-20(a)–(b) and OQ-8 | A final grant ceiling may have a nonzero microstep; the raw core NET is different from participant NET. Clamping without checking actual work can mis-tag external input or stall on invalid core NET. | Specify a full-width retained authorization and a separately selected, stable scalar execution candidate. Define a supported external-only path or the required scheduler change. Extend UT-9 for stalls, NET changes and a ceiling preceding the next candidate. |

## 3 Hardware coordination implementation guide (revision 1)

<a id="tab-3"></a>

**Table 3 — # and Passage.**

| # | Passage | Problem and evidence | Proposed change |
|---|---|---|---|
| IG-1 | DEC-11, rationale "The baseline commits cited by the ZCU106 note do not form a proven build" | Narrowed by DN-3. | Append the source-identity finding; keep the decision open until the port's revision is recorded. |
| IG-2 | Stage table, B3 and B4 | The project definition adds a hardware participant with a local known-input engine (B-HW1, B-HW2). It reuses Track A's adapter and buffers but is a different policy engine; it should not be read as B3 or B4. | Add a note to the stage table distinguishing B-HW from B3/B4. |
| IG-3 | P0 trace model and A53 reference coordinator | A standalone trace shim does not provide a board service or a verified data/control binding. | Distinguish the trace oracle from the board coordinator. Add a gate for serialized event ingestion, lifecycle, visible admission and grant dispatch before the first hardware federation. |

## 4 micro-LF on FPGA — Project Definition (Claude Doc, revision 31)

The independent review ([`REVIEW_codex.md` Section 5](https://github.com/perkusic/lingua-franca-docs/blob/f429e437f5d6db66269ec88ef4f09a73e3458adf/micro-lf/validation/REVIEW_codex.md)) proposes replacement text for findings B1–B11. In addition:

<a id="tab-4"></a>

**Table 4 — # and Passage.**

| # | Passage | Problem | Proposed change |
|---|---|---|---|
| PD-1 | Validation example, V3: "Tag 500 is resolved at about 551 ms, after the sensors stop at 550 ms." | The review traced the cause: in the independent Linux debug run, a TCP disconnect wakes the waiting scheduler and the closed channel is skipped by the acquisition scan. | "Preserve the historical measurement. Add that the independent Linux debug run reproduced the release at physical elapsed time 550.445 ms via TCP disconnect, before the 600 ms acquisition deadline; it did not receive a value or stop message to resolve this input." |
| PD-2 | "Finding W_min": "the sweep finds it at 61 ms on the test host" | 61 ms is the smallest successful **tested** value on that host (three attempts per value); it is not a bound. | Use "smallest successful tested value" and state the attempts and host. |
| PD-3 | "The program": "has not been compiled, because … Maven Central … was not reachable" and the code listing using `tardy` as a state name | Superseded: `tardy` is reserved; the corrected program compiles with the `ulf/` compiler of the same reactor-uc commit, and the generated C differs from the hand-written C in queue capacities, keep-alive and TCP setup. | Replace the listing with the corrected source and add the generated-versus-hand-written table from the review. |
| PD-4 | Pass criteria table: "Tardy count … 1 in V2" as part of PASS | The shutdown check does not test the tardy count (review B4). | Move tardy count, presence trace and process exits to the harness (`reproduce.py`). |

## 5 Repository

<a id="tab-5"></a>

**Table 5 — # and Item.**

| # | Item | Proposed action |
|---|---|---|
| RP-1 | Branch `codex/review-micro-lf` (`f429e43`) changes `Validate3.ulf`, the validation README, the micro-LF design note, the tutorial, its RTI companion and their skeletons, and adds a CI workflow | Review and merge (or cherry-pick) so that `main` holds the corrected program and the review; the project definition cites the branch commit until then. |
| RP-2 | Source manifest | Create at M-A0 with every commit and tool version, and Software Heritage identifiers for the pinned commits. |
