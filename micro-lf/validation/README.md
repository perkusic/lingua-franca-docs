# Three-federate validation example (micro-LF on FPGA project)

Two sensor federates (periods 100 ms and 150 ms) feed one fusion federate that sums their values and
logs, for every tag, the lag between physical time and the tag. The example exercises the micro-LF
decision: an input that is already known, a wait that expires after `@maxwait`, an input that becomes
known through a later message, and a late message handled by the `tardy` handler.

| File | What it is |
|---|---|
| `Validate3.ulf` | the micro-LF program, compiled and tested on Linux with the pinned uLFC |
| `sensor.c`, `fusion.c` | hand-written reference against `lf-lang/reactor-uc` at commit `df7cfea`; see the review for differences from generated C |
| `CMakeLists.txt` | builds `sensor_a`, `sensor_b`, `sensor_b_slow`, `fusion_w30`, `fusion_w100` and a sweep `fusion_w55` … `fusion_w70` |
| `run.sh` | starts fusion, then the two sensors, and prints the fusion log |
| `reproduce.py` | runs all cases, keeps per-process logs, and compares process exits and logical traces against the reference outcomes |

## Build and run (Linux)

From the `lingua-franca-docs` repository root, with JDK 17, CMake, a C compiler and Python 3:

    git clone https://github.com/lf-lang/reactor-uc.git ../reactor-uc
    git -C ../reactor-uc checkout --detach df7cfeae7083509621b8f5080c066051112e2500
    git -C ../reactor-uc submodule update --init --recursive
    export REACTOR_UC_PATH="$(cd ../reactor-uc && pwd)"
    cmake -S micro-lf/validation -B build/validation && cmake --build build/validation
    python3 micro-lf/validation/reproduce.py manual build/validation evidence
    "$REACTOR_UC_PATH/ulf/gradlew" -p "$REACTOR_UC_PATH/ulf" assemble :cli:lfc:assemble
    python3 micro-lf/validation/reproduce.py generated "$REACTOR_UC_PATH/ulf/build/install/lf-cli/bin/lfc" evidence

The sweep compares against the reference host's outcomes. A different classification around 60–61 ms
requires inspecting the per-process logs; that boundary is not a portable timing guarantee.
The independent Linux measurements and the explanation of tag 500 are in [REVIEW_codex.md](REVIEW_codex.md).

## Results (9 November 2026)

Linux 6.18 x86-64 (a cloud container with 2 cores, not the ZCU106), GCC 13.3, CMake 3.28, TCP over
loopback, clock synchronization disabled (all federates read the same host clock).
All three configurations print `8 reactions … sum a 21, sum b 10: PASS`; V1 and V3 report 0 tardy
and V2 reports 1, handled at tag (300 ms, 1). Sweeping `@maxwait` with sensor B 60 ms late at tag
300 ms: W = 55, 59 and 60 ms give one tardy event in each of three runs; W = 61, 62, 65 and 70 ms give
none, and tag 300 is processed at a lag of 60.6–60.9 ms. On this host W_min is therefore 61 ms:
the 60 ms injected sender lag plus about 0.6 ms of runtime and loopback latency.
