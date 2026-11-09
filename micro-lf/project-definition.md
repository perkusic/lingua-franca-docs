# micro-LF on FPGA — Project Definition

Nov 9, 2026 · @Angelo

> Snapshot exported from the Claude Doc [micro-LF on FPGA — Project Definition](https://claude.ai/artifact/Qh4BhWciPT1kwVdbuEcbh9) at revision 31 (9 November 2026). The five diagrams are interactive widgets and appear here only as placeholders; the live doc is the source of truth.

## Summary

The project runs micro Lingua Franca (micro-LF, the `reactor-uc` runtime) as a federation inside one AMD ZCU106 board, with the federates communicating through the programmable logic instead of TCP. It then moves the parts of the runtime that profiling shows to matter into hardware, one block at a time. It is Track B of the project's [implementation guide](https://perkusic.github.io/lingua-franca-docs/program/implementation_guide.html#tab-stages).

**The question it answers.** On one chip there is a single oscillator, so the clock error between federates is zero, and a channel in the fabric can have a bounded latency. Under those conditions, how small can the safe waiting bound W of decentralized coordination be, and for which federations does that make a central coordinator (the HW-RTI of Track A) unnecessary? The measured answer is the input to open question [OQ-7](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html#tab-open-questions) of the [HW-RTI specification](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html), due before its milestone [M-4](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html#tab-milestones).

**What exists at the end.**

- micro-LF federations running on the ZCU106 over a fabric channel (`HwLinkChannel`, an AXI4-Stream FIFO) with a fabric time base (a 64-bit counter), with no change to generated code.
- Measurements of waiting bound, latency, jitter and CPU cost, over TCP and over the fabric channel, from the same traces.
- A written answer to OQ-7.
- Depending on two gates: a bare-metal soft-core port of micro-LF, and the first blocks of a hardware input engine (HW-STP), each checked against a software reference model.

Reaction bodies stay in software throughout; synthesizing them is out of scope.

## Motivation and context

micro-LF coordinates a federation without a central runtime infrastructure (RTI). A federate decides by itself when it may process a tag g: if every input already has a `last_known_tag` ≥ g, it proceeds; otherwise it waits until physical time g.t + W, where W is the largest `@maxwait` among the unresolved inputs. The wait is correct only if W covers three terms: the sender's lag (how late it produces an output after the tag's time), the channel latency L and the clock error E between federates.

On general-purpose machines none of the three terms has a proven bound, which is why reactor-c also offers centralized coordination, and why the [HW-RTI project](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html) proposes a central coordinator in hardware. On one FPGA two of the three change: every federate can read the same counter (E = 0 by construction), and a dedicated FIFO or a time-divided network-on-chip bounds L. Only the sender's lag remains, set by reaction execution time and, on the ARM cores, by Linux.

The HW-RTI specification records this as risk [R-9](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html#tab-risks) and open question [OQ-7](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html#tab-open-questions), and the implementation guide ([DEC-13](https://perkusic.github.io/lingua-franca-docs/program/implementation_guide.html#dec-13)) requires OQ-7 to be answered in writing, with measurements, before HW-RTI milestone [M-4](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html#tab-milestones). This project produces those measurements, and then tests whether hardware in the federate's input path (the HW-STP proposed in the [micro-LF design note](https://perkusic.github.io/lingua-franca-docs/micro-lf/micro_lf_fpga_design_note.html)) reduces the part of W that the platform contributes.

Two results are equally useful: a small measured W shows that decentralized coordination suffices for a defined class of on-chip federations; a large one, or one dominated by sender lag, shows where centralized coordination remains necessary.

## Objectives and research questions

| ID | Objective or question | Success criterion |
| --- | --- | --- |
| O-1 | Run unmodified micro-LF federations on the ZCU106, first over TCP and then over a fabric channel | The reference programs produce the same logical traces over both channels |
| O-2 | RQ-1: what is the smallest W with no safe-to-process violation, per channel and load? | W\_min measured on the A53 over TCP and over the fabric channel, with the distribution of sender lag and channel latency |
| O-3 | RQ-2: which share of W comes from the platform (interrupts, copying, scheduling) rather than from the application? | Arrival-to-admission latency and CPU time per message measured and set against W\_min |
| O-4 | RQ-3: for which federations is central coordination still necessary on chip? | A written answer to OQ-7 naming the classes (CPU federates with unbounded lag, zero-delay cycles, others found) |
| O-5 | RQ-4: does a bare-metal soft core reduce W\_min relative to Linux on the A53? | Same measurements on the soft core; done only if gate G2 passes |
| O-6 | RQ-5: does a hardware input engine reduce admission latency and jitter enough to matter? | Each hardware block matches its software reference model and its effect on W\_min is measured; done only if gate G3 passes |

## Scope

The project covers the software federation on the board, the fabric channel and time base, the measurements, and the gated hardware blocks. Everything that belongs to the centralized track or to synthesis of reactions stays outside.

**In scope**

- micro-LF (`lf-lang/reactor-uc`, pinned commit) with its POSIX platform on the Cortex-A53 cores of the ZCU106 under PetaLinux.
- A `HwLinkChannel` that implements micro-LF's `NetworkChannel` interface over an AXI4-Stream FIFO, reached from Linux user space through UIO and polled ([DEC-09](https://perkusic.github.io/lingua-franca-docs/program/implementation_guide.html#dec-09)).
- A platform time source that reads one free-running 64-bit fabric counter over AXI4-Lite, with clock synchronization off ([DEC-10](https://perkusic.github.io/lingua-franca-docs/program/implementation_guide.html#dec-10)).
- Instrumentation, test programs and measurement scripts, and the OQ-7 answer.
- Gated: a bare-metal port of micro-LF to one soft core (Platform interface: five functions and a mutex), and the first HW-STP blocks in the order of the design note: framing and payload movement, then connection state and tag comparison, then timing and tardiness logic.

**Out of scope**

- The HW-RTI, the reactor-chisel adapter and the centralized protocol (Track A).
- Stages B3 and B4 of the [guide](https://perkusic.github.io/lingua-franca-docs/program/implementation_guide.html#tab-stages) (the known-input and bounded-waiting branches on the Track A participant infrastructure): they depend on Track A stages P3 and P4 and belong to a later project.
- Synthesis of reaction bodies, network transports leaving the board, and multi-board federations.
- The event-queue offload, unless profiling at gate G3 shows it dominates.
- Changes to the `reactor-uc` runtime beyond the new channel and platform files; defects found ([DEC-14](https://perkusic.github.io/lingua-franca-docs/program/implementation_guide.html#dec-14)) are reported upstream, not patched in place.

## System architecture

In phase P2 every federate is a Linux process on the A53 cores; the programmable logic supplies only what the federates share, a channel and a time base. Nothing in the generated code changes: the channel and the time source are selected when the federate is built.

*[Diagram in the live doc: ZCU106 partition · phase P2, with the gated additions dashed]*

The filled boxes are built in P2. The dashed boxes are added only if their gate passes: a soft core running a federate without an operating system (G2), and an input engine between the FIFOs and a federate (G3).

## Requirements

Each requirement names the test that discharges it; the tests are described under Verification and evaluation. Requirements MR-07 to MR-09 apply only if their gate passes.

| ID | Requirement | Test |
| --- | --- | --- |
| MR-01 | Federated micro-LF programs generated by `ulfc` build and run on the A53 cores under PetaLinux, with the POSIX platform and the TCP channel, without changes to generated code. | T-1 |
| MR-02 | `HwLinkChannel` implements the `NetworkChannel` interface over one AXI4-Stream FIFO per direction and connection: ordered, lossless, payload up to the runtime bound (832 bytes at `df7cfea`). | T-2, T-3 |
| MR-03 | `send_blocking` returns when the frame is in the FIFO; a full FIFO blocks the sender up to a configured timeout and then reports failure. No frame is dropped silently. | T-3 |
| MR-04 | Connection setup, reset and reconnection over `HwLinkChannel` follow the same state sequence as the TCP channel, so the startup protocol is unchanged. | T-2 |
| MR-05 | Physical time comes from one 64-bit fabric counter read over AXI4-Lite by every federate; clock synchronization is disabled; the counter's resolution is one fabric clock cycle. | T-4 |
| MR-06 | Each federate logs, per message: arrival, admission, the decision (KNOWN, when every input is known, or EXPIRED, when the deadline g.t + W passed), reaction start, and tardy events, timestamped with the fabric counter. | T-5 |
| MR-07 | (gate G2) A bare-metal micro-LF platform for one soft core implements `get_physical_time`, `wait_until`, `wait_for`, `wait_until_interruptible`, `notify` and the mutex, using the fabric counter and `HwLinkChannel`. | T-1, T-2 on the soft core |
| MR-08 | (gate G3) Each HW-STP block has a software reference transition model; the block and the model give identical results on every test trace. | T-6 |
| MR-09 | (gate G3) The hardware keeps KNOWN and EXPIRED as distinct results, never advances `last_known_tag` for a dropped event, and keeps the intended tag of a late message (DEC-14). | T-6 |
| MR-10 | Logical traces are identical across repeated runs, across channels and across platforms, for programs whose inputs do not come from physical actions. | T-2 |
| MR-11 | Every result is reproducible from the repository: pinned commits of `reactor-uc`, `ulfc`, Vivado and PetaLinux, the bitstream build script and the measurement scripts. | T-7 |
| MR-12 | Resource use (LUT, FF, BRAM) and achieved clock of every fabric block are reported from post-route results. | T-7 |

## Work plan

The project measures first and builds hardware only where the measurements point. Phases P0 to P2 are the guide's stages B1 and B2 and are enough to answer OQ-7; P3 and P4 are each started only if their gate passes, and may run in parallel.

*[Diagram in the live doc: work plan · six phases, three gates]*

| Phase | Content | Exit criterion |
| --- | --- | --- |
| P0 | Pin `reactor-uc`, `ulfc`, Vivado and PetaLinux; build the PetaLinux image; choose the test programs | T-1 builds on a clean machine (D-1) |
| P1 | micro-LF on the A53 with the POSIX platform and TCP loopback; instrumentation | T-1, T-5 pass; B-TCP measured (D-2) |
| P2 | Fabric FIFOs and counter; `HwLinkChannel` and fabric-time platform | T-2, T-3, T-4 pass; B-FIFO measured; OQ-7 answered (D-3, D-4) |
| P3 | Bare-metal micro-LF on one soft core, same channel and counter | T-1, T-2 pass on the soft core; B-SOFT measured (D-5) |
| P4 | HW-STP blocks in order: framing and payload movement (P4a), input state and tag comparison (P4b), timing and tardiness logic (P4c); event queue only if profiling shows it dominates | T-6 passes for each block; its effect on admission latency and W\_min measured (D-6) |
| P5 | Final measurements, report, upstream issue reports | D-7 |

## Verification and evaluation

Correctness is judged on logical traces; performance on timestamps from the fabric counter. A configuration that is faster but admits late events is not reported as better than one that does not: every result states the coordination policy and the number of violations next to the latency.

| Test | What it checks | Method |
| --- | --- | --- |
| T-1 | Programs build and run (MR-01, MR-07) | The three-federate validation example (V1 to V3), the micro-LF examples of the tutorial (`src`/`dst` with `tardy`, a pipeline, a feedback pair with a delay) plus the `FederatedMaxWait` tests of `reactor-uc`; outputs compared with the reference runs |
| T-2 | Channel equivalence (MR-02, MR-04, MR-10) | The same programs over TCP loopback and over `HwLinkChannel`; logical traces (tag, port, value, violation) must match event for event |
| T-3 | Channel failure paths (MR-02, MR-03) | Full FIFO, sender timeout, receiver reset, reconnection, maximum payload; no silent loss, failures reported |
| T-4 | Time base (MR-05) | Counter monotonic and identical across federates; read latency from user space measured |
| T-5 | Instrumentation (MR-06) | Log completeness on synthetic traces with known arrival times |
| T-6 | Hardware blocks (MR-08, MR-09) | Differential tests against the software reference model, in simulation and on the board; boundary cases: equal tags, timeout and arrival in the same cycle, queue full, reset |
| T-7 | Reproducibility and resources (MR-11, MR-12) | Clean rebuild from the repository on a second machine; post-route reports archived |

**Metrics**, each reported as median, 99th percentile and maximum over a stated number of runs and a stated background load:

- W\_min: the smallest `@maxwait` with no safe-to-process violation, found by sweeping W downward.
- Sender lag: physical time of sending minus the message's tag time.
- Channel latency L: send to arrival.
- Arrival-to-admission latency and CPU time per message.
- Number of tardy events as a function of W.
- End-to-end latency, sensor event to actuator reaction, for one example pipeline.
- LUT, FF, BRAM and achieved clock for each fabric block.

**Baselines.** B-TCP: the same federation over TCP loopback with the A53 clock. B-FIFO: over `HwLinkChannel` with the fabric counter. If gate G2 passes, B-SOFT: the soft-core variant. Comparisons with the HW-RTI use its own measurements ([KPI-1, KPI-2](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html#tab-kpis)) and are made only for the same program and the same safety property.

## Validation example: three federates

One small program validates every phase: two sensor federates with different periods feed one fusion federate, which sums their values and logs how late each tag is processed. It exercises the whole micro-LF decision: an input already known, an input made known by a later message, a wait that expires, and a message that arrives after its tag. The program was run on 9 November 2026 as hand-written C against `reactor-uc` at `df7cfea`, on Linux over TCP loopback; every expected value below is a measured one from that run, and the board phases must reproduce the logical results.

### Structure

*[Diagram in the live doc: validation example · three federates, two connections]*

Only fusion has inputs, so only fusion ever waits. Its two inputs share one reaction, and `@maxwait` applies to both: at each tag, fusion proceeds when both inputs are known or when W has passed since the tag's time.

### The program

The program, its C equivalent and the scripts are in [`micro-lf/validation/`](https://github.com/perkusic/lingua-franca-docs/tree/main/micro-lf/validation) of the project repository. `Validate3.ulf` is the micro-LF source; it follows the syntax of the `reactor-uc` tests but has not been compiled, because the micro-LF compiler `ulfc` is built with Gradle and needs Maven Central, which was not reachable. `sensor.c` and `fusion.c` are the same three federates written by hand with the macros the compiler generates; they are what was run.

```c
target uC

// Sends 1, 2, 3, ... every `period`. At firing number `slow_at` it first spends
// `slow_by` of physical time: that one message leaves late, with its tag unchanged.
reactor Sensor(period: time = 100 ms, slow_at: int = 0, slow_by: time = 0 ms) {
  output out: int
  timer t(0, period)
  state n: int = 0
  reaction(t) -> out {=
    self->n++;
    if (self->n == self->slow_at) {
      env->platform->wait_for(env->platform, self->slow_by);
    }
    lf_set(out, self->n);
  =}
}

// Reacts to either input, sums what it receives and logs the lag
// (physical time minus the tag's time) at which each tag is processed.
reactor Fusion {
  input a: int
  input b: int
  state count: int = 0
  state tardy: int = 0
  state sum_a: int = 0
  state sum_b: int = 0
  reaction(a, b) {=
    self->count++;
    if (lf_is_present(a)) self->sum_a += a->value;
    if (lf_is_present(b)) self->sum_b += b->value;
    printf("fusion: tag %lld ms, a %s, b %s, lag %lld us\n",
           (long long)(env->get_elapsed_logical_time(env) / MSEC(1)),
           lf_is_present(a) ? "present" : "absent",
           lf_is_present(b) ? "present" : "absent",
           (long long)((env->get_physical_time(env) - env->get_logical_time(env)) / USEC(1)));
  =} tardy {=
    self->tardy++;
    if (lf_is_present(a)) self->sum_a += a->value;
    if (lf_is_present(b)) self->sum_b += b->value;
    printf("fusion: TARDY message handled at %lld ms\n",
           (long long)(env->get_elapsed_logical_time(env) / MSEC(1)));
  =}
  reaction(shutdown) {=
    int ok = self->count == 8 && self->sum_a == 21 && self->sum_b == 10;
    printf("fusion: %d reactions, %d tardy, sum a %d, sum b %d: %s\n",
           self->count, self->tardy, self->sum_a, self->sum_b, ok ? "PASS" : "FAIL");
  =}
}

@platform("native")
@timeout(550 ms)
federated reactor {
  sa = new Sensor(period = 100 ms)
  sb = new Sensor(period = 150 ms)
  // sb = new Sensor(period = 150 ms, slow_at = 3, slow_by = 60 ms)   // V2, V3
  @maxwait(30 ms)                                                     // 100 ms in V3
  fusion = new Fusion()
  sa.out -> fusion.a
  sb.out -> fusion.b
}
```

The shutdown check holds in every configuration: whatever the timing, fusion must see the six values of sensor A (sum 21) and the four of sensor B (sum 10), and must react at eight tags. A late message is not lost; it is counted by the `tardy` handler instead.

### Running it

**Phase P1, on a Linux host or on the A53.** Build the C equivalent against `reactor-uc` and start fusion first, because it listens for both sensors (ports 8921 and 8922):

```bash
git clone --recurse-submodules https://github.com/lf-lang/reactor-uc.git
cd reactor-uc && git checkout df7cfea && git submodule update --init && cd ..
export REACTOR_UC_PATH=$PWD/reactor-uc
cmake -B build -S validation && cmake --build build
cd build
../validation/run.sh fusion_w30  sensor_b         # V1
../validation/run.sh fusion_w30  sensor_b_slow    # V2
../validation/run.sh fusion_w100 sensor_b_slow    # V3
```

**Phase P2, on the board.** The same three programs, each a Linux process on the A53, built with two changes: the `TcpIpChannel` of each connection is replaced by `HwLinkChannel` on its FIFO pair, and the platform's physical time is read from the fabric counter. No line of the federates' own code changes. Once `ulfc` builds, `Validate3.ulf` replaces the hand-written C in both phases.

### V1: the baseline run

*[Diagram in the live doc: V1 · measured decision times, W = 30 ms, B on time]*

**How to read Figure V1.** Each dot on the upper two lines is a message, placed at its tag. On the fusion line, a vertical mark is the instant the reaction ran, in physical time measured from start; a shaded bar before it is time spent waiting for an input that has no message at that tag. The letters say which inputs were present.

**What Figure V1 shows.** Where both sensors fire (0 and 300 ms), fusion runs within half a millisecond, because both messages arrive and both inputs become known. Where only one sensor fires, fusion cannot know that the other input is absent until W has passed, so it runs about 30 ms late. This is the price of decentralized coordination without null messages: absence is learned by waiting. Table V1 gives the measured values.

| Tag (ms) | Inputs present | Measured lag (ms) | Why |
| --- | --- | --- | --- |
| 0 | a, b | 0.4 | both known |
| 100 | a | 30.2 | b expired after W |
| 150 | b | 30.3 | a expired after W |
| 200 | a | 30.2 | b expired after W |
| 300 | a, b | 0.4 | both known |
| 400 | a | 30.2 | b expired after W |
| 450 | b | 30.3 | a expired after W |
| 500 | a | 30.1 | b expired after W |

Table V1 — Lags measured in V1 (lag = physical time of the reaction minus the tag). Result: 8 reactions, 0 tardy, sum a = 21, sum b = 10, PASS.

### V2 and V3: a late message

In V2 and V3, Sensor B delays its third message (tag 300) by 60 ms, so it reaches fusion at about 360 ms of physical time. The two runs differ only in W.

*[Diagram in the live doc: V2 vs V3 · the same late message at tag 300]*

**How to read Figure V2/V3.** Each panel zooms on tag 300. The dashed line is B's late message reaching fusion; the shaded bar is the waiting budget W; the vertical marks are what fusion did and when.

**What Figure V2/V3 shows.** With W = 30 ms (V2), fusion gives up at 330 ms and runs tag 300 with b absent. When B's message arrives at about 360 ms its tag is already in the past, so the runtime hands it to the tardy handler at (300 ms, 1): the value is not lost, but the reaction that should have seen it did not. With W = 100 ms (V3), the message arrives inside the budget, b becomes known, and fusion runs once with both inputs at about 360.6 ms. Neither run is wrong: V2 trades a late answer for timeliness and reports it, V3 trades timeliness for completeness.

V3 also shows a side effect of a large W. At tags with one sensor, fusion often learns that the other input is absent from that sensor's next message, whose later tag proves that nothing is pending: tag 100 becomes known when B's tag-150 message arrives (lag about 50.5 ms). Tag 200 has no such message within the budget and expires after the full 100 ms. Tag 500 is resolved at about 551 ms, after the sensors stop at 550 ms. Table V2 summarises both runs.

| Run | W (ms) | Tag 300 | Lag at 300 (ms) | Tardy | Other lags (ms) | Result |
| --- | --- | --- | --- | --- | --- | --- |
| V2 | 30 | b absent, then tardy at (300, 1) | 30.1 (tardy 60.7) | 1 | as V1 | PASS |
| V3 | 100 | a, b present | 60.6 | 0 | 50.3–50.6; 100.3 at tag 200; \~51 at 500 | PASS |

Table V2 — Measured outcome of the late-message runs. PASS means 8 reactions and correct sums (a = 21, b = 10), and the tardy count expected for the run.

### Finding W\_min

W\_min is the smallest `@maxwait` at which the late message of tag 300 is still waited for. With sensor B 60 ms late, the sweep finds it at 61 ms on the test host: the 60 ms of sender lag plus about 0.6 ms of runtime and loopback latency. Each value was run three times; the results did not vary between runs.

| `@maxwait` (ms) | Tardy events (3 runs) | Tag 300 processed with | Lag at tag 300 (ms) |
| --- | --- | --- | --- |
| 55 | 1, 1, 1 | b absent | 55.2 |
| 59 | 1, 1, 1 | b absent | 59.2 |
| 60 | 1, 1, 1 | b absent | 60.1–60.2 |
| 61 | 0, 0, 0 | b present | 60.6–60.7 |
| 62 | 0, 0, 0 | b present | 60.6–60.7 |
| 65 | 0, 0, 0 | b present | 60.7–60.9 |
| 70 | 0, 0, 0 | b present | 60.6–60.7 |

Below W\_min the lag follows W (the federate gives up at the deadline); above it the lag stops at the arrival time of the message, 60.6 ms, however large W is. This is the procedure of metric W\_min in phases P1 and P2: the same sweep on the A53 over TCP, then over `HwLinkChannel`, with the injected lag set to zero to measure what the platform alone contributes.

Test host: Linux 6.18 on x86-64 (a cloud container with 2 cores, not the ZCU106), GCC 13.3, CMake 3.28, TCP over loopback, clock synchronization off, since all federates read the same host clock.

### Pass criteria and what a failure points to

The example passes in a phase when all of the checks hold. Logical results (columns a, b, tardy, sums) must match exactly; lags are compared within a tolerance stated with the measurement.

| Check | Expected | A failure points to |
| --- | --- | --- |
| Startup completes and the three federates agree on a start time | Fusion prints its first line at tag 0 | Connection setup of the channel (MR-04), or the startup protocol on this topology |
| Shutdown line | `8 reactions` and `sum a 21, sum b 10: PASS` in V1, V2, V3 | A lost or duplicated message: channel ordering or loss (MR-02, MR-03) |
| Tardy count | 0 in V1 and V3, 1 in V2, at tag (300 ms, 1) | The decision itself: `last_known_tag`, the max over unresolved inputs, or the deadline g.t + W |
| Presence per tag | The a and b columns of the V1 table, identical over TCP and `HwLinkChannel` | A channel that reorders or merges frames (T-2) |
| Lag pattern | Near 0 at tags 0 and 300; about W elsewhere in V1; about 50 ms where a later message decides (V3) | Physical time: a counter read wrongly, or a clock not shared (MR-05) |
| W\_min | 60 ms plus the platform's own latency | The quantity being measured, not a failure: report it with its distribution |

## Deliverables

| ID | Deliverable | Phase |
| --- | --- | --- |
| D-1 | Source manifest (pinned commits and tool versions), PetaLinux image and build scripts | P0 |
| D-2 | Measurement report over TCP on the A53 (B-TCP) | P1 |
| D-3 | Fabric design: AXI4-Stream FIFOs and 64-bit counter, with `HwLinkChannel` and the fabric-time platform code | P2 |
| D-4 | Measurement report over the fabric channel (B-FIFO) and the written OQ-7 answer | P2 |
| D-5 | Bare-metal soft-core platform and its measurement report (B-SOFT) | P3, if G2 |
| D-6 | HW-STP blocks: RTL, software reference models, differential tests, resource reports | P4, if G3 |
| D-7 | Final report and upstream issue reports for the `reactor-uc` behaviours of DEC-14 | P5 |

## Risks and mitigations

| Risk | Effect | Mitigation |
| --- | --- | --- |
| Sender lag on the A53 under Linux dominates W, hiding the benefit of E = 0 and a bounded L | RQ-1 answered, but with a large W\_min | Measure lag separately (MR-06); gate G2 tests a bare-metal soft core, where lag depends on the reaction alone |
| `send_blocking` over a FIFO behaves differently from TCP (completion, back-pressure, reset) | Traces differ between channels, or federations hang | Define "sent" and failure behaviour first (MR-03); T-2 and T-3 before any measurement |
| UIO polling costs CPU time and adds latency | The fabric channel looks worse than it is | Report CPU time per message; an interrupt-driven variant if polling dominates |
| Questionable `reactor-uc` behaviours (knowledge advancing after a dropped event, payload release on failure paths, the joining-time write) | Hardware copies a defect | Reproduce each in software, decide per case (DEC-14), report upstream; MR-09 |
| Startup round count not exact on cyclic neighbor graphs | Startup fails or waits on cyclic test programs | Test a cyclic program in T-1; report if reproduced |
| The HW-STP gives no measurable gain because W is set by the application | Phase P4 produces RTL without benefit | Gate G3: start P4 only if admission latency is a relevant share of W\_min |
| `reactor-uc` changes upstream during the project | Results tied to an outdated runtime | Pin the commit (MR-11); re-run T-1 and T-2 on a newer commit before the final report |
| The `ulfc` code generator is built with Gradle and needs Maven Central; the validation example is hand-written C for that reason | Generated code is not exercised; T-1 rests on hand-written federates | Build `ulfc` in P0 with network access and pin it (MR-11); regenerate the example from `Validate3.ulf` and compare its trace with the hand-written one |

## Resources, platform and open decisions

**Platform.** AMD ZCU106 board, Zynq UltraScale+ XCZU7EV-2FFVC1156E: four Cortex-A53 cores under PetaLinux, and programmable logic with 230,400 LUTs, 460,800 flip-flops, 312 block RAMs and 96 UltraRAMs, the same board as Track A.

**Software.** `reactor-uc` at `df7cfea` (7 September 2026) as the starting point, `ulfc` (the micro-LF code generator, built with Gradle from a pinned commit; it needs Maven Central at build time), Vivado and PetaLinux in versions to be fixed in P0.

**People and time.** Not yet defined. As a planning estimate only, not from the sources: P0 two weeks, P1 three, P2 six, P3 four, each P4 block four to eight, P5 three. P0 to P2 alone, about eleven weeks, answer OQ-7.

**Open decisions**

- [ ] Which `reactor-uc` commit to pin: `df7cfea` or a newer one checked again against the behaviours of DEC-14.
- [ ] Polling or interrupts for `HwLinkChannel` under UIO (DEC-09 proposes polling).
- [ ] Which soft core for P3: MicroBlaze V, a generic RISC-V core, or FlexPRET (which already has a micro-LF platform but no channel).
- [ ] Which target applications define "small enough" for W in the OQ-7 answer.
- [ ] Whether the PS reads the fabric counter directly or a copy kept in the A53's own timer, and the cost of each.

## References

Project documents, in the repository [perkusic/lingua-franca-docs](https://github.com/perkusic/lingua-franca-docs):

1. [*Hardware coordination implementation guide*](https://perkusic.github.io/lingua-franca-docs/program/implementation_guide.html), rev. 1: decisions [DEC-09 to DEC-14](https://perkusic.github.io/lingua-franca-docs/program/implementation_guide.html#tab-decision-register); [stages B1 to B4](https://perkusic.github.io/lingua-franca-docs/program/implementation_guide.html#tab-stages).
2. [*micro-LF FPGA design note*](https://perkusic.github.io/lingua-franca-docs/micro-lf/micro_lf_fpga_design_note.html), rev. 1: channel backends, the proposed hardware/software boundary, verification before RTL and the development sequence.
3. [*HW-RTI Project Specification*](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html), rev. 0.2: [open questions, OQ-7](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html#tab-open-questions), [risks, R-9](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html#tab-risks), [KPI-1 and KPI-2](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html#tab-kpis), [milestones](https://perkusic.github.io/lingua-franca-docs/hw-rti/HW-RTI_project_specification.html#tab-milestones).
4. [*Lingua Franca tutorial*](https://perkusic.github.io/lingua-franca-docs/tutorial/lingua_franca_tutorial.html), [Lesson 16](https://perkusic.github.io/lingua-franca-docs/tutorial/lingua_franca_tutorial.html#s16) (micro-LF: federated execution without an RTI) and [Section 5.3](https://perkusic.github.io/lingua-franca-docs/tutorial/lingua_franca_tutorial.html#s5-3); the [micro-LF example sources](https://github.com/perkusic/lingua-franca-docs/tree/main/tutorial/examples/micro-lf).
5. [*Viabilidade de Lingua Franca em FPGA*](https://claude.ai/code/artifact/57eb05fe-d1c5-40da-ac6a-79122f6fd6d6) (feasibility study of 15 October 2026, in Portuguese).

Source code:

6. [`lf-lang/reactor-uc`](https://github.com/lf-lang/reactor-uc/tree/df7cfeae7083509621b8f5080c066051112e2500) on GitHub at commit `df7cfea` (full hash `df7cfeae7083509621b8f5080c066051112e2500`): `include/reactor-uc/platform.h`, `include/reactor-uc/network_channel.h`, `src/environments/federated_environment.c`, `src/federated.c`.
