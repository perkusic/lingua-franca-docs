#!/bin/sh
# Run one configuration of the three-federate validation example on one Linux host.
# Usage: ./run.sh <fusion executable> <sensor B executable>     (from the build directory)
#   V1: ./run.sh fusion_w30  sensor_b
#   V2: ./run.sh fusion_w30  sensor_b_slow
#   V3: ./run.sh fusion_w100 sensor_b_slow
# Fusion listens on 127.0.0.1:8921 (sensor A) and :8922 (sensor B), so it starts first.
F=${1:-fusion_w30}; B=${2:-sensor_b}
./$F > fusion.log 2>&1 & FP=$!
sleep 0.3
./sensor_a > sensor_a.log 2>&1 &
./$B > sensor_b.log 2>&1 &
wait $FP; sleep 0.2
grep -v WARN fusion.log
