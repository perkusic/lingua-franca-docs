Expanded technical design note \| 8 October 2026

# 1 Recommended first implementation

Begin with centralized coordination using the existing project HW-RTI
and the ZCU106 reactor-chisel port. The decisive advantage is
architectural fit: Codesign already provides a grant-controlled
execution boundary and NET/LTC-like progress signals. A centralized
baseline allows the team to establish tag representation, completion,
transport ordering and participant isolation before introducing another
local advancement policy.

This is a recommendation for the current project, not a claim that
centralized coordination is always faster or simpler. It is conditional
on the project HW-RTI having a usable, verified policy for the chosen
initial topology. If that policy is unavailable, build and validate a
reference coordinator for that scope first; do not connect unverified
grant logic to the FPGA and treat successful execution as proof of
safety.

Use two hardware participants, a static acyclic topology with explicit
connection delays, bounded channels, and final grants. Keep startup and
diagnostics on the Arm processing system. Defer provisional grants,
zero-delay cyclic dependencies and mixed coordination modes until their
semantics are required and tested. If the intended application
fundamentally needs those features, the restricted baseline is a
verification scaffold rather than an application-complete solution.

Develop the decentralized case as a second policy branch on the same
participant and transport infrastructure. Start with explicit input
knowledge or absence information, then add bounded waiting only after
deriving the timing assumptions and the violation response. This keeps
the comparison focused on coordination rather than on two unrelated
hardware implementations.

## Scope and evidence

The existing board port is taken as complete. The original Codesign
interface findings are based on inspected source files \[1–8\]. LF and
micro-LF documentation provides semantic reference points \[9–12\]. The
architectures, records and flowcharts below are proposed integration
artifacts; they do not assert that Codesign already implements
federation or that the project HW-RTI has been audited here.

The note preserves the original Codesign findings and expands them with
both coordination choices, their motivation, code reuse, implementation
gates, example traces and validation criteria. No canonical project
document is modified.

# 2 Why coordination is needed

Independent participants can compute concurrently, yet they must agree
on the logical order of events when that order affects reactions and
state. A receiver facing an input with no value at a candidate tag must
distinguish known absence from a value that is merely delayed. A local
timer can otherwise trigger a reaction before causally relevant remote
data becomes visible.

Coordination answers when a participant may commit to a tag, which
inputs belong to that tag, and what completion means for downstream
participants. Transport answers how information moves. Central placement
of decisions does not inherently require central routing of all
payloads; conversely, peer-to-peer payload exchange does not establish
decentralized advancement.

## Motivation for centralization

A coordinator provides a single place to evaluate progress against
topology and connection delays. It offers a useful reference
architecture when sender execution and channel delays vary and the
initial research goal is faithful logical-time ordering. Global traces
can make incorrect grants, missing reports and unresolved dependencies
easier to diagnose. Central hardware can also share policy resources
rather than replicating them beside every participant.

Its costs include report traffic, grant arbitration, dependence on the
coordinator, and potential serialization. Conservative coordination can
wait indefinitely for stalled upstream participants. Causal correctness
alone provides no deadline guarantee; schedulability still requires
bounds on computation, transport, arbitration and release timing.

## Motivation for decentralization

Local decisions can remove the per-tag grant round trip, keep traffic
close to the computation, and suit larger or physically distributed
deployments. Bounded waiting can make a deliberate availability choice
when a peer is slow or unavailable. The cost is replicated knowledge
state and a stronger obligation to justify timeouts, preserve ordering,
and handle late data.

Distribution does not automatically provide fault tolerance or lower
latency. Infinite waits preserve input alignment but can block progress;
short waits can create tardy inputs. A shared clock on one FPGA
simplifies clock alignment, but does not bound sender lag, queueing or
downstream backpressure. These costs must be measured or bounded for the
selected workload.

| **Decision dimension** | **Centralized initial baseline**                 | **Decentralized branch**                                 |
|------------------------|--------------------------------------------------|----------------------------------------------------------|
| Authority to advance   | Verified grant policy                            | Local knowledge and waiting policy                       |
| Unknown input          | Resolve by protocol before final execution       | Wait or assume absence under explicit policy             |
| Main integration risk  | Progress and grant meanings do not match         | Local policy and timing assumptions are incomplete       |
| Failure behavior       | Coordinator or missing progress can stall        | Peer loss can stall or trigger a configured violation    |
| Expected first use     | Reference implementation on current ZCU106 setup | Subsequent experiment in local progress and availability |

# 3 What the original Codesign implementation provides

<img
src="program/sources/lf_hardware_coordination_zcu106_media/media/image1.png"
style="width:6.65in;height:3.20031in"
alt="Figure 1. Original hardware and software coordination path. This is a component execution protocol, not an identified multi-federate RTI." />

Figure 1. Original hardware and software coordination path. This is a
component execution protocol, not an identified multi-federate RTI.

CodesignSwEmitter selects the Cpp software target.
CodesignFpgaWrapperTransformation generates a wrapper that uses software
elapsed logical time as a hardware grant, issues WRITE, polls
logicalTagComplete, forwards outputs and issues READ. It queries
nextEventTag to schedule hardware-originating work in the software
runtime \[6,7\].

CoordinationIO exposes NET, TAG, LTC and shutdown fields.
TriggerGenerator connects its token-queue NET, clock and
coordination-valid signal to Scheduler. The scheduler selects local,
external or combined execution, then sends a decoupled execute command
\[1,2\].

| **Code location**                  | **Meaning for integration**                                                   |
|------------------------------------|-------------------------------------------------------------------------------|
| Scheduler.scala                    | Readiness predicates and the execution gate; TAG greater than NET restriction |
| Reaction.scala                     | reactionEnable/reactionDone and sIdle/sRunning/sDone token handling           |
| CodesignTopReactor.scala           | WRITE-linked validity, top-level ports, DMA and LtcHandler                    |
| Wrapper transformation in compiler | Software transaction sequence and next-event scheduling                       |
| Tag.scala                          | Scalar 32-bit tags and sentinel representation                                |

reactionDone defaults to true and must be controlled for multi-cycle
bodies. LtcHandler tracks output adapter completion rather than directly
reducing every reactionDone. Array outputs wait for both reactor and DMA
completion. No-output completion uses input-port readiness; that proxy
needs validation for a new federation contract \[3–5\].

# 4 Centralized block architecture

<img
src="program/sources/lf_hardware_coordination_zcu106_media/media/image2.png"
style="width:6.65in;height:3.86531in"
alt="Figure 2. Proposed central coordination. The ordered data plane exposes admission information to the policy; payload routing and grant placement remain separate choices." />

Figure 2. Proposed central coordination. The ordered data plane exposes
admission information to the policy; payload routing and grant placement
remain separate choices.

Each participant contains a reactor-chisel group and an adapter that
owns ingress state, execution transactions, egress buffers and progress
reports. HW-RTI receives coherent participant state and issues grants
according to the project topology and delay policy. The Arm side
configures and observes the experiment rather than granting every tag.

For the first experiment, prefer a route whose delivery ordering is
visible to the coordinator, such as a simple ordered router with
explicit admission acknowledgement. A control-only coordinator with
direct peer data is also possible, but then in-flight data and knowledge
closure require a proved protocol. An egress enqueue alone does not
establish downstream input visibility.

Central authority permits advancement only when the verified policy
excludes causally earlier missing input or otherwise provides the
required input closure. A simplistic minimum of NET values is not an RTI
algorithm. Progress reports must cover the event sources, connection
delays, executing work and outbound information required by that policy.

# 5 Centralized participant flow

<img
src="program/sources/lf_hardware_coordination_zcu106_media/media/image3.png"
style="width:6.65in;height:5.48625in"
alt="Figure 3. Proposed central participant flow. The guard combines policy permission with local transaction conditions; it is not a replacement for the RTI algorithm." />

Figure 3. Proposed central participant flow. The guard combines policy
permission with local transaction conditions; it is not a replacement
for the RTI algorithm.

Maintain a retained grant with explicit validity and request identity.
Select a real pending candidate covered by that grant. Latch one input
snapshot and issue execution exactly once. A permission to advance is
not an instruction to fabricate an event.

Before reporting completion, establish the required output publication
point and retain all buffer ownership until downstream acknowledgement
or an equivalent release. The next report must not advertise a later
candidate while the previous transaction still has unreported causally
relevant work.

# 6 Centralized protocol and implementation obligations

## A small transaction sequence

| **Step** | **Participant A**                        | **Coordinator and ordered fabric**     | **Participant B**                 |
|----------|------------------------------------------|----------------------------------------|-----------------------------------|
| 1        | Reports candidate g                      | Evaluates initial progress             | Reports its pending state         |
| 2        | Executes under permission                | Maintains dependency information       | Waits for input closure           |
| 3        | Produces output for destination tag h    | Admits data or tracks ordered delivery | Accepts tagged input h            |
| 4        | Reports completion after required commit | Evaluates downstream safety for h      | Receives final permission for h   |
| 5        | Refreshes candidate                      | Processes coherent progress            | Executes h and reports completion |

Here h is derived by the agreed connection-delay and tag transformation.
Zero delay is not assumed to preserve a scalar timestamp without
microstep implications. This sequence is an explanatory trace for the
restricted baseline, not the full coordinator transition function.

## Required adapter changes

Separate grant value, grant validity and grant acknowledgement from
input publication. The original WRITE mechanism couples them. Define a
command handshake that cannot lose repeated commands; SwCommand
currently recognizes changes in command value. Preserve a consumption
acknowledgement equivalent to READ for output token release.

Resolve the current scheduler assertion rejecting TAG greater than NET
except FOREVER. Either narrow the adapter to an explicitly proved
execution transaction model, retaining a wider grant separately, or
extend the scheduler after checking the RTI contract. Do not silently
discard the assertion or truncate grant values.

NET from the local token queue is not automatically the participant NET
needed by an RTI. The adapter may need to combine admitted external
events, local timers, physical actions and executing-state restrictions.
Similarly, publish LTC only when reaction work and outbound commitments
satisfy the project invariant.

## Safety and progress boundaries

Use final grants first on the verified acyclic scope. A valid zero-delay
feedback network can require provisional coordination or finer knowledge
exchange; restricting such networks is a scope decision, not proof that
all feedback is invalid. Deadlock and safety must be tested separately.
Shutdown and reset need coherent epochs so old messages and reports
cannot affect a new run.

# 7 Decentralized block architecture

<img
src="program/sources/lf_hardware_coordination_zcu106_media/media/image4.png"
style="width:6.65in;height:4.23937in"
alt="Figure 4. Proposed local coordination engines. Replicated state replaces per-tag central grants; shared startup or clock services can remain." />

Figure 4. Proposed local coordination engines. Replicated state replaces
per-tag central grants; shared startup or clock services can remain.

Place a local policy engine beside each reactor-chisel group. Incoming
data and knowledge information update a connection table and tagged
ingress queue. The engine chooses a candidate, evaluates input knowledge
and physical release, and either waits or admits execution under the
selected policy. Egress publications carry enough ordering information
for peers to update knowledge safely.

Use the same participant buffers, connection maps and execution
interface as the centralized baseline where possible. Only one engine
owns advancement at a time. Switching policies requires a quiescent
protocol or a verified handover; two simultaneous authorities are
unsafe.

The LF documentation distinguishes conventional decentralized targets,
which may retain RTI services for lifecycle and clock synchronization,
from micro-LF, which uses peer coordination without an RTI \[9\]. The
proposed hardware design can retain an Arm startup service without
making that service a per-tag coordinator.

# 8 Decentralized admission flow

<img
src="program/sources/lf_hardware_coordination_zcu106_media/media/image5.png"
style="width:6.65in;height:6.27594in"
alt="Figure 5. Conceptual decentralized admission flow. Exact deadline derivation, absence rules and update priority must be extracted from the selected reference implementation." />

Figure 5. Conceptual decentralized admission flow. Exact deadline
derivation, absence rules and update priority must be extracted from the
selected reference implementation.

Knowledge may be established by a present event, explicit absence, or a
valid ordered progress bound. A larger arriving tag proves earlier
knowledge only if the sender and channel contract justifies that
inference. Check all required inputs for the candidate; do not replace
per-input state with the last packet timestamp.

A receive update racing with timeout must be resolved through an atomic
snapshot/commit protocol. If the input state version changes, recompute
readiness before execution. Late input classification uses the intended
tag and completed execution frontier, with a defined equality and
microstep rule.

# 9 Waiting policy and timing discussion

The decentralized branch should first use known-input progress,
including explicit absence or lower-bound information where necessary.
This establishes correctness without guessing a finite waiting
parameter. It can stall if upstream peers fail to produce the knowledge
needed for progress. Cyclic waiting dependencies require a protocol
argument, lookahead, or an explicit source of progress.

Only then introduce finite waiting as an application policy. The current
LF documentation describes connection-level maxwait and absence-related
attributes, while older documentation uses STP offsets \[9,10\]. These
versions are not interchangeable with the examined Chisel runtime or
unpinned reactor-uc headers. Extract the full policy from one chosen
commit before translating it to hardware.

## Timing budget proposal

For a required input at destination time t, establish an upper bound on
the physical arrival time as observed by the receiver. The analysis must
include source release and execution lag, all queue and transport delay,
timestamp conversion and connection delay, receiver admission delay,
clock error, and any margin used by the design. Microsteps are logical
ordering components, not physical durations.

Define B_i(g) as a derived upper bound on the receiver-clock arrival
time for input i relevant to candidate g, and D_i(g) as the policy
instant at which absence may be assumed. To claim no tardiness from that
assumption, require D_i(g) to be no earlier than B_i(g), under the exact
boundary rule. This is a verification condition, not the reactor-uc
max_wait equation.

If queueing or backpressure is unbounded, a finite B_i(g) has not been
established. Average latency, a typical FPGA clock frequency, or a
shared clock do not establish it. A smaller waiting allowance may still
be intentional, but it changes the availability/consistency contract and
requires explicit late-data handling.

## Violation response

Define whether a tardy input triggers a fault, drops a value, invokes a
hardware handler, switches to a declared degraded mode, or requests
application recovery. Rollback is not supplied by the original Codesign
interface and is outside the initial design. Do not let ordinary
reaction execution silently consume stale data after a timeout.

# 10 Worked timing example

<img
src="program/sources/lf_hardware_coordination_zcu106_media/media/image6.png"
style="width:6.65in;height:3.6575in"
alt="Figure 6. A delayed input distinguishes a conservative wait from a finite absence assumption. This is an illustrative experiment, not a measured performance result." />

Figure 6. A delayed input distinguishes a conservative wait from a
finite absence assumption. This is an illustrative experiment, not a
measured performance result.

Consider a receiver with a local timer at logical time 100 us and one
remote logical input relevant to that tag. The remote value arrives at
physical time 108 us. The example assumes a coherent time reference, a
supported tag representation and no microstep complication.

In the centralized case, the final permission is withheld until the
required data/knowledge closure is satisfied. The receiver can then
process the logical tag 100 us after physical arrival. Physical lateness
has not changed the intended logical tag, although an application
deadline may already have been missed.

In a decentralized case whose verified policy allows a 10 us waiting
allowance, arrival at 108 us can satisfy knowledge before the
illustrative deadline at 110 us. The receiver need not wait to the
deadline once all required conditions are satisfied.

With a 5 us allowance, the engine may assume absence at physical time
105 us, if that is permitted by the policy. The timer reaction executes
without the remote value. Arrival at 108 us is then tardy relative to
that committed decision. This case demonstrates a contract change, not a
faster equivalent execution.

If the engine has two unknown inputs, it must account for both input
policies and their dependencies. A single global offset chosen from this
example is insufficient. Include a delayed-input and an absence-input
test, because receiving no message is not inherently evidence that an
input is absent.

# 11 Reuse of LF Codesign and micro LF

| **Source**                                | **Centralized route**                                                                | **Decentralized route**                                                                 |
|-------------------------------------------|--------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------|
| reactor-chisel and Codesign               | Reuse reactions, token protocols, execution gate and I/O adapters; adapt TAG/NET/LTC | Reuse execution and transport primitives; add a local admission policy                  |
| Conventional LF RTI and federated runtime | Reference grant safety, topology, completion, startup and relevant control messages  | Reference supported local coordination semantics and late-data behavior                 |
| micro-LF/reactor-uc                       | Use bounded buffers and channel patterns where useful; no central policy to import   | Reference per-input state, max_wait handling, input knowledge and scheduler transitions |
| Existing project HW-RTI                   | First authority for the verified initial scope                                       | Comparison baseline and possible reusable arithmetic or RAM blocks                      |

Reuse algorithms and contracts separately from reuse of source code. C
and C++ runtime structs, mutexes and pointer-based queues are not Chisel
modules. Their behavior can serve as a reference model; hardware
requires explicit storage, ownership, arbitration and bounded capacity.
Existing Chisel modules can be reused more directly, subject to version
and semantic compatibility.

The reactor-uc federation header exposes per-input delay, type,
last_known_tag, max_wait and payload_pool \[11\]. These identify
candidate state for a local engine. They do not establish its scheduler
decision function or prove correspondence between max_wait and newer
source-language annotations. Preserve logical versus physical connection
behavior and trace every knowledge update.

The event header retains intended_tag alongside scheduled event metadata
\[12\]. This supports the design requirement to preserve intended
logical identity for violation detection, even if an arrival is admitted
after its intended execution frontier.

## Corrections carried into both routes

NetworkChannel backends are transport alternatives rather than TCP
emulation. Application payload serialization is distinct from
protocol-envelope encoding; protobuf is not universally required for
port values. Peer transport is not synonymous with decentralized
advancement. A bounded wait is not proof that late input is impossible.
These distinctions remain relevant even when all endpoints are hardware.

# 12 Common interface and implementation artifacts

| **Artifact**                   | **Required fields or decisions**                                                   | **Purpose**                                     |
|--------------------------------|------------------------------------------------------------------------------------|-------------------------------------------------|
| Source manifest                | Compiler, runtime, submodule, board port and HW-RTI commits; tool versions         | Reproducible baseline                           |
| Topology manifest              | Participant and connection IDs, logical/physical type, delay, payload schema       | Same configuration for both policies            |
| Tag contract                   | Time width, microsteps, epoch, unit, sentinels, order and overflow                 | Remove incompatible scalar/pair interpretations |
| Event record proposal          | Destination, intended tag, presence, payload handle/length, sequence and epoch     | Atomic admission and owned payload lifetime     |
| Progress/grant record proposal | Participant, event frontier, valid flags, grant kind and report version            | Coherent authority and progress exchange        |
| Transition model               | State, guards, updates, failure paths and same-cycle priority                      | Reference for RTL and differential testing      |
| Trace schema                   | Physical sample, participant, event tag, commit, execute, completion and violation | Explain mismatches and measure latency          |

These are contract outlines, not a frozen wire format or register map.
Data structures need separate validity for no event and sentinel tags.
Grant permissions and progress frontiers must retain their distinct
meanings. The trace should distinguish a candidate query from execution
and computation completion from transport commit.

## Source incompatibilities to close

The inspected reactor-chisel Tag is a 32-bit UInt, whereas reaction
registers and software conversions have different widths. Its NEVER and
FOREVER construction and unsigned comparisons need review. No explicit
microstep pair appears in that Tag definition. Resolve these differences
before using LF tag comparators or accepting project HW-RTI tags \[8\].

The runtime baseline is d94dd2d272ce46145ac13c3c6a141bc2ba9dfca1. The
compiler baseline is f60c83c5fbf0f0c26c780c28e58e50c0fc60a608, whose
reactor-chisel submodule is 6c8aecd0211acc4c1e58d1cdd9ae950880a25204.
Pin a matched combination from the working ZCU106 port rather than
combining these observations as a proven build.

# 13 Implementation stages and acceptance gates

<img
src="program/sources/lf_hardware_coordination_zcu106_media/media/image7.png"
style="width:6.65in;height:4.3225in"
alt="Figure 7. Recommended development sequence. Shared adapters and tests precede divergent policy work." />

Figure 7. Recommended development sequence. Shared adapters and tests
precede divergent policy work.

| **Stage** | **Deliverable**                                 | **Acceptance gate**                                                   |
|-----------|-------------------------------------------------|-----------------------------------------------------------------------|
| S0        | Matched source manifest and semantic subset     | Working port reproduces the original example                          |
| S1        | Instrumented original Codesign trace            | WRITE, execute, LTC and READ agree with expectations                  |
| S2        | Common participant adapter and bounded channels | Atomic admission, ownership and backpressure tests pass               |
| S3        | Two-participant centralized baseline            | Final grants and completion match the reference on the declared scope |
| S4        | Distributed known-input branch                  | No premature absence; live progress demonstrated on its scope         |
| S5        | Bounded wait and later advanced grants          | Timing assumptions, violations and policy semantics tested            |

Start with source/sink behavior, then add a join with two upstream
inputs and a local timer. Positive-delay feedback can be a later case
after its liveness argument is established. Do not activate PTAG or
mixed-policy handover merely because the simpler trace passes.

# 14 Validation and the decision to distribute

| **Scenario**                | **Centralized check**                                            | **Distributed check**                                                |
|-----------------------------|------------------------------------------------------------------|----------------------------------------------------------------------|
| Delayed input               | No premature final execution; data visibility ordered with grant | Known-input wait or deliberate timeout; correct tardy classification |
| Explicit absence            | Absence/closure justifies the grant                              | Absence or progress information is committed before release          |
| Multiple inputs             | Grant does not bypass an unresolved dependency                   | All required input policies are evaluated                            |
| Same time and microsteps    | Tag ordering and supported grant transitions                     | Knowledge and expiry handle the declared microstep rules             |
| Multi-cycle reaction / DMA  | LTC does not precede required output commit                      | Local completion and peer knowledge remain ordered                   |
| Full buffers / stalled peer | Correct backpressure; no false progress                          | No silent loss; configured waiting/violation behavior                |
| Reset and boundary arrival  | Epoch protection and defined update priority                     | Atomic timeout/arrival decision and intended-tag retention           |

Use a reference transition model and compare logical traces rather than
demanding equal clock-cycle timing from different policies. Where finite
waiting changes behavior intentionally, compare each trace against its
own declared contract and report violations. Never count a
timeout-driven omission as an equivalent throughput improvement.

Measure event-to-execution latency, physical lag, coordination traffic,
queue occupancy, wait time, tardy count, sustained throughput, logic/RAM
usage and timing closure. Record where coordinator arbitration or local
policy scanning is the bottleneck. Use identical reaction computation,
topology and channel capacity when comparing policies; isolate
deliberate timing-policy differences.

## Decision gate

Proceed toward distributed coordination when the central baseline is
semantically stable and measurements or deployment requirements justify
removing per-tag central dependence. Confirm that needed timing bounds
are defensible, the local knowledge protocol is complete, and the
application has an acceptable response to late or missing data. If these
conditions are absent, central coordination remains the more defensible
baseline for this project.

For this ZCU106 effort, the first milestone is a verified central
federation of two hardware participants. The second milestone is a local
known-input policy using the same infrastructure. The third is a
measured, explicitly bounded distributed experiment. This order
maximizes reuse and provides a clear reference when the two policies
differ.

# 15 Source map and references

[\[1\]
TriggerGenerator.scala](https://github.com/erlingrj/reactor-chisel/blob/d94dd2d272ce46145ac13c3c6a141bc2ba9dfca1/src/main/scala/TriggerGenerator.scala#L15)

CoordinationIO; scheduler wiring at lines 123–131; repeated-tag
assertion at lines 135–137.

[\[2\]
Scheduler.scala](https://github.com/erlingrj/reactor-chisel/blob/d94dd2d272ce46145ac13c3c6a141bc2ba9dfca1/src/main/scala/Scheduler.scala#L20)

SchedulerIO, readiness predicates, grant restriction and execution state
machine.

[\[3\]
CodesignTopReactor.scala](https://github.com/erlingrj/reactor-chisel/blob/d94dd2d272ce46145ac13c3c6a141bc2ba9dfca1/src/main/scala/CodesignTopReactor.scala#L69)

LtcHandler at line 76; top at 126; validity at 175; TopLevelPorts at
220; array output completion at 623.

[\[4\]
Reaction.scala](https://github.com/erlingrj/reactor-chisel/blob/d94dd2d272ce46145ac13c3c6a141bc2ba9dfca1/src/main/scala/Reaction.scala#L145)

reactionDone and reactionEnable; reactionMain and output-token
publication.

[\[5\]
Reactor.scala](https://github.com/erlingrj/reactor-chisel/blob/d94dd2d272ce46145ac13c3c6a141bc2ba9dfca1/src/main/scala/Reactor.scala#L130)

isIdle implementation and reactorMain.

[\[6\]
CodesignFpgaWrapperTransformation.java](https://github.com/erlingrj/lingua-franca/blob/f60c83c5fbf0f0c26c780c28e58e50c0fc60a608/core/src/main/java/org/lflang/ast/CodesignFpgaWrapperTransformation.java#L365)

Main wrapper generation; TAG and WRITE at 408–410; LTC polling at 412;
READ and NET at 421–428.

[\[7\]
CodesignSwEmitter.kt](https://github.com/erlingrj/lingua-franca/blob/f60c83c5fbf0f0c26c780c28e58e50c0fc60a608/core/src/main/kotlin/org/lflang/generator/codesign/CodesignSwEmitter.kt#L72)

Cpp target selection and generated software project.

[\[8\]
Tag.scala](https://github.com/erlingrj/reactor-chisel/blob/d94dd2d272ce46145ac13c3c6a141bc2ba9dfca1/src/main/scala/Tag.scala#L4)

Scalar hardware Tag type, width and sentinels.

[\[9\] LF distributed execution
documentation](https://www.lf-lang.org/docs/next/writing-reactors/distributed-execution/)

Nightly reference for centralized/decentralized coordination and
micro-LF differences. Documentation semantics require commit alignment.

[\[10\] LF 0 8 distributed execution
documentation](https://www.lf-lang.org/docs/0.8.0/writing-reactors/distributed-execution/)

Earlier STP-offset description; retained to explain terminology
evolution, not as the new hardware policy.

[\[11\] reactor uc federated connection
header](https://dev.micro-lf.org/federated_8h_source.html)

Per-input connection state and serializer/deserializer interfaces;
generated API is not a pinned implementation.

[\[12\] reactor uc event
header](https://dev.micro-lf.org/event_8h_source.html)

Event metadata, intended_tag and payload pools.

Source inspection and documentation access: 8 October 2026. The existing
HW-RTI implementation and the ZCU106 port are project context, not
audited source in this note. Proposed architectures and flowcharts must
be checked against their exact contracts before RTL implementation.
