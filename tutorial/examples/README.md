# Lingua Franca examples

Every program here is printed in `lingua_franca_tutorial.html` and was compiled and run with
`lfc` 0.13.0 on Linux (Java 21, gcc, CMake, Python 3.11) on 28 September 2026. Each comes in
C and in Python (`…Py.lf`), except `fixpoint.py`, a plain Python script for the appendix (§15). Installation and tools: §14 of the tutorial.

Put the files you want in a `src/` folder, then:

    lfc src/Hello.lf
    ./bin/Hello

| Example | Files | Shows |
|---|---|---|
| 2.1 | `Hello.lf`, `HelloPy.lf` | a timer and logical time |
| 2.2 | `Levels.lf`, `LevelsPy.lf` | Figure 2: reactions ordered by declaration and dependency |
| 2.3 | `Microsteps.lf`, `MicrostepsPy.lf` | superdense time: zero delay vs positive delay |
| 3.1 | `TwoFeds.lf`, `TwoFedsPy.lf` | the smallest federation; what `lfc` generates |
| 4.1 | `Cycle.lf`, `CyclePy.lf` | the S–A–B–C federation of §4.13, traced (PTAG, TAG, DNET) |
| 5.1 | `Decentralized.lf`, `DecentralizedPy.lf` | decentralized coordination: `after`, `@maxwait`, `tardy`, `deadline` |
| 7.1 | (a one-line change to `Cycle.lf`) | a causality loop rejected by the compiler |
| 8.1 | `Deadline.lf`, `DeadlinePy.lf` | a physical action and a deadline |
| 12.1 | `FeedbackCentralized.lf`, `FeedbackCentralizedPy.lf` | lab: grants, no PTAG |
| 12.2 | `FeedbackZeroDelay.lf`, `FeedbackZeroDelayPy.lf` | lab: a constructive zero-delay cycle, PTAGs |
| 12.3 | `FeedbackDecentralized.lf`, `FeedbackDecentralizedPy.lf` | lab: clocks instead of grants; the comment at the top gives the tardy-message recipe |
| 15.1 | `fixpoint.py` | appendix: tag addition checked against `lf_tag_add`, then the min-delay closure and the port iteration (present, absent, not triggered) as fixed points; a teaching model, not the runtime (plain Python 3: `python3 src/fixpoint.py`) |
| 16.1 | `micro-lf/Tardy.ulf` (+ `src.c`, `dst.c`, `CMakeLists.txt`) | micro-LF without an RTI: `@maxwait`, `tardy`, a late message; **not compiled with ulfc** (Maven Central unreachable), runtime behaviour checked with the hand-written C equivalent against reactor-uc `df7cfea`; see `micro-lf/README.md` |

Federated programs start with `./bin/<Name>`, a launcher that runs the RTI and every federate.
If a run fails with "Port 15045 is not available", an earlier RTI still holds the port: find it with
`lsof -i :15045`, check with `ps -o pid,args -p <pid>` that it is the stale run, and stop only that
process with `kill <pid>` (a blanket `pkill RTI` would also stop another experiment's RTI).
