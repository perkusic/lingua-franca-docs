Use the attached **Centralized and Decentralized Coordination for LF Hardware** document as the technical basis for an implementable research project on the AMD ZCU106. Produce a standalone project definition in English that an engineer can use to plan, implement, and validate the work.

Treat the document as a design proposal. Separate what has been verified in the original Codesign source, what the existing ZCU106 port already provides, what the Linux experiment demonstrated, and what remains to be built or tested on the board. If a repository, source revision, or board artifact is unavailable, identify the resulting uncertainty. Do not invent commands, interfaces, measurements, or test results.

### Project structure

Keep these two tracks distinct:

- **Track A — Centralized HW-RTI:** Use the existing reactor-chisel ZCU106 port to build a first federation with two independently controlled hardware participants, a static acyclic topology, and final TAG grants. Use the original Codesign `TAG`, `NET`, `LTC`, `WRITE`, and `READ` interface as the integration starting point. Explain which semantics need adaptation, especially tag representation, grant validity, input closure, and completion when outputs are buffered or delayed.
- **Track B — Decentralized micro-LF:** Use reactor-uc as the software reference. Begin with the A53 Linux baseline over TCP loopback, then compare it with a fabric channel and shared time base. For a later hardware participant, establish progress using explicit input knowledge before adding finite waits and tardy handling. Keep this later experiment distinct from the initial A53 work.

Recommend Track A as the first **hardware federation milestone**, conditional on having a verified grant policy for the chosen topology. Track B’s Linux and A53 reference work can proceed independently. Explain what evidence would justify moving from centralized to decentralized advancement. Peer-to-peer transport alone does not make coordination decentralized.

### Deliverables

1. **Project definition:** Motivation, research questions, scope, assumptions, and explicit exclusions. Make clear why the original Codesign wrapper is useful without treating it as an existing federation-wide RTI.
2. **Architecture and implementation plan:** Block diagrams for both tracks, a flowchart for each advancement policy, and one sequence diagram following a tagged value from sender execution through receiver admission and completion. Identify who owns each decision and buffer.
3. **Reuse map:** For original LF/reactor-c, reactor-chisel/Codesign, micro-LF/reactor-uc, and the project HW-RTI, distinguish code that can be reused directly, behavior that needs an adapter, and algorithms that should serve only as references.
4. **Requirements and acceptance:** Map each requirement to a test. Define milestones and measurable gates before using them. For the first milestone, require a trace showing that two participants execute under final grants, resolve presence or absence correctly, and do not report completion before the required output commitment.
5. **Validation and decision criteria:** Cover delayed data, explicit absence, microsteps, backpressure, reset, and tardiness. Separate semantic checks from performance measurements. Explain how results inform OQ-7, and what additional timing or application argument is needed to answer it.
6. **Risks and next actions:** Identify open decisions, the evidence needed to close each one, and the first implementation tasks. Distinguish planning estimates from measured results.

If the independent review is attached, incorporate its findings: the 61 ms value was the smallest successful **tested** wait on that Linux host; TCP disconnect explains the release of tag 500 in the hand-written V3 run; and generated C differs from the hand-written reference in queue capacities, keep-alive, and TCP setup. Do not present Linux loopback results as ZCU106 measurements or a worst-case timing guarantee.

Cite source claims with repository, full commit, file path, and a pinned link where available. Deliver a reviewable Markdown project definition. Preserve the attached document and canonical project sources; list proposed changes separately.