# Three-federate validation example (micro-LF on FPGA project)

Two sensor federates (periods 100 ms and 150 ms) feed one fusion federate that sums their values and
logs, for every tag, the lag between physical time and the tag. The example exercises the micro-LF
decision: an input that is already known, a wait that expires after `@maxwait`, an input that becomes
known through a later message, and a late message handled by the `tardy` handler.

| File | What it is |
|---|---|
| `Validate3.ulf` | the program in micro-LF syntax (not compiled here: `ulfc` needs Maven Central to build) |
| `sensor.c`, `fusion.c` | the same federates written by hand against `lf-lang/reactor-uc` at commit `df7cfea`, with the macros the generator emits |
| `CMakeLists.txt` | builds `sensor_a`, `sensor_b`, `sensor_b_slow`, `fusion_w30`, `fusion_w100` and a sweep `fusion_w55` … `fusion_w70` |
| `run.sh` | starts fusion, then the two sensors, and prints the fusion log |

## Build and run (Linux)

    git clone --recurse-submodules https://github.com/lf-lang/reactor-uc.git
    cd reactor-uc && git checkout df7cfea && git submodule update --init && cd ..
    export REACTOR_UC_PATH=$PWD/reactor-uc
    cmake -B build -S validation && cmake --build build
    cd build
    ../validation/run.sh fusion_w30  sensor_b         # V1
    ../validation/run.sh fusion_w30  sensor_b_slow    # V2
    ../validation/run.sh fusion_w100 sensor_b_slow    # V3

## Results (9 November 2026)

Linux 6.18 x86-64 (a cloud container with 2 cores, not the ZCU106), GCC 13.3, CMake 3.28, TCP over
loopback, clock synchronization disabled (all federates read the same host clock).
All three configurations print `8 reactions … sum a 21, sum b 10: PASS`; V1 and V3 report 0 tardy
and V2 reports 1, handled at tag (300 ms, 1). Sweeping `@maxwait` with sensor B 60 ms late at tag
300 ms: W = 55, 59 and 60 ms give one tardy event in each of three runs; W = 61, 62, 65 and 70 ms give
none, and tag 300 is processed at a lag of 60.6–60.9 ms. On this host W_min is therefore 61 ms:
the 60 ms injected sender lag plus about 0.6 ms of runtime and loopback latency.
