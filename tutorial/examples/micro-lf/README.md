# micro-LF example (Lesson 16)

`Tardy.ulf` is Example 16.1 of `lingua_franca_tutorial.html`: two federates, `src` and `dst`,
with no RTI. `src` holds message 3 back for 100 ms; `dst` waits at most `@maxwait` for its input.

**How it was checked (15 October 2026).** The micro-LF compiler `ulfc` is built with Gradle from
`lf-lang/reactor-uc`, which needs Maven Central; that server was not reachable from the environment
used, so `Tardy.ulf` has **not** been compiled. Its syntax follows the integration tests of
`reactor-uc` at commit `df7cfea` (`test/lf/src/federated/FederatedMaxWait*.ulf`).
The runtime behaviour printed in the tutorial comes from `src.c` and `dst.c`: the same two federates
written by hand against the `reactor-uc` runtime at `df7cfea`, with the macros the generator emits,
built and run on Linux (x86-64, GCC 13.3, CMake).

## Run the C equivalent (Linux or macOS)

    git clone --recurse-submodules https://github.com/lf-lang/reactor-uc.git
    cd reactor-uc && git checkout df7cfea && git submodule update --init && cd ..
    export REACTOR_UC_PATH=$PWD/reactor-uc
    cmake -B build -S micro-lf && cmake --build build
    ./build/src & sleep 0.3; ./build/dst_short; wait     # @maxwait(50ms): tardy at (400 ms, 1)

| executable | stands for |
|---|---|
| `src` | federate `src` (TCP server, clock-sync grandmaster) |
| `src_sparse` | `src` sending only odd counts |
| `dst_short` | `dst` with `@maxwait(50ms)` and a `tardy` handler |
| `dst_long` | `dst` with `@maxwait(150ms)` |
| `dst_nohandler` | `dst` with `@maxwait(50ms)` and no handler |

Start `src` (or `src_sparse`) first; it listens on 127.0.0.1:8911.

## Compile the .ulf (not verified here)

    $REACTOR_UC_PATH/ulf/bin/ulfc-dev src/Tardy.ulf
    ./bin/Tardy
