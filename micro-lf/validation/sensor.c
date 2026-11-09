/* Hand-written equivalent of a `Sensor` federate of Validate3.ulf (reactor-uc df7cfea, POSIX, TCP).
   Compile-time parameters: NAME, PERIOD_MS, SLOW_AT, SLOW_MS, PORT_NUM. */
#include "reactor-uc/platform/posix/tcp_ip_channel.h"
#include "reactor-uc/schedulers/dynamic/scheduler.h"
#include "reactor-uc/reactor-uc.h"
#include <sys/socket.h>
#ifndef SLOW_AT
#define SLOW_AT 0
#endif
#ifndef SLOW_MS
#define SLOW_MS 0
#endif

LF_DEFINE_TIMER_STRUCT(Sensor, t, 1, 0)
LF_DEFINE_TIMER_CTOR(Sensor, t, 1, 0)
LF_DEFINE_REACTION_STRUCT(Sensor, r, 1)
LF_DEFINE_REACTION_CTOR(Sensor, r, 0, NULL, NULL)
LF_DEFINE_OUTPUT_STRUCT(Sensor, out, 1, int)
LF_DEFINE_OUTPUT_CTOR(Sensor, out, 1)

typedef struct {
  Reactor super;
  LF_TIMER_INSTANCE(Sensor, t);
  LF_REACTION_INSTANCE(Sensor, r);
  LF_PORT_INSTANCE(Sensor, out, 1);
  int n;
  LF_REACTOR_BOOKKEEPING_INSTANCES(1, 2, 0);
} Sensor;

LF_DEFINE_REACTION_BODY(Sensor, r) {
  LF_SCOPE_SELF(Sensor);
  LF_SCOPE_ENV();
  LF_SCOPE_PORT(Sensor, out);
  self->n++;
  if (self->n == SLOW_AT) env->platform->wait_for(env->platform, MSEC(SLOW_MS));
  lf_set(out, self->n);
  printf("%s: sent %d, tag %lld ms, lag %lld us\n", NAME, self->n,
         (long long)(env->get_elapsed_logical_time(env) / MSEC(1)),
         (long long)((env->get_physical_time(env) - env->get_logical_time(env)) / USEC(1)));
}

LF_REACTOR_CTOR_SIGNATURE_WITH_PARAMETERS(Sensor, OutputExternalCtorArgs *out_external) {
  LF_REACTOR_CTOR_PREAMBLE();
  LF_REACTOR_CTOR(Sensor);
  LF_INITIALIZE_REACTION(Sensor, r, NEVER);
  LF_INITIALIZE_TIMER(Sensor, t, MSEC(0), MSEC(PERIOD_MS));
  LF_INITIALIZE_OUTPUT(Sensor, out, 1, out_external);
  LF_TIMER_REGISTER_EFFECT(self->t, self->r);
  LF_PORT_REGISTER_SOURCE(self->out, self->r, 1);
  self->n = 0;
}

int serialize_int(const void *v, size_t sz, unsigned char *buf) { memcpy(buf, v, sizeof(int)); return sizeof(int); }

LF_DEFINE_FEDERATED_OUTPUT_CONNECTION_STRUCT(Sensor, out, int)
LF_DEFINE_FEDERATED_OUTPUT_CONNECTION_CTOR(Sensor, out, int, 0)

typedef struct {
  FederatedConnectionBundle super;
  TcpIpChannel channel;
  LF_FEDERATED_OUTPUT_CONNECTION_INSTANCE(Sensor, out);
  LF_FEDERATED_CONNECTION_BUNDLE_BOOKKEEPING_INSTANCES(0, 1);
} LF_FEDERATED_CONNECTION_BUNDLE_TYPE(Sensor, Fusion);

LF_FEDERATED_CONNECTION_BUNDLE_CTOR_SIGNATURE(Sensor, Fusion) {
  LF_FEDERATED_CONNECTION_BUNDLE_CTOR_PREAMBLE();
  TcpIpChannel_ctor(&self->channel, "127.0.0.1", PORT_NUM, AF_INET, false);
  LF_FEDERATED_CONNECTION_BUNDLE_CALL_CTOR();
  LF_INITIALIZE_FEDERATED_OUTPUT_CONNECTION(Sensor, out, serialize_int);
}

LF_DEFINE_STARTUP_COORDINATOR_STRUCT(Federate, 1, 8);
LF_DEFINE_STARTUP_COORDINATOR_CTOR(Federate, 1, 2, 8, JOIN_IMMEDIATELY);
LF_DEFINE_SHUTDOWN_COORDINATOR_STRUCT(Federate, 8);
LF_DEFINE_SHUTDOWN_COORDINATOR_CTOR(Federate, 2, 8);
LF_DEFINE_CLOCK_SYNC_STRUCT(Federate, 1, 8);
LF_DEFINE_CLOCK_SYNC_DEFAULTS_CTOR(Federate, 1, 8, false);

typedef struct {
  Reactor super;
  LF_CHILD_REACTOR_INSTANCE(Sensor, sensor, 1);
  LF_FEDERATED_CONNECTION_BUNDLE_INSTANCE(Sensor, Fusion);
  LF_FEDERATE_BOOKKEEPING_INSTANCES(1, 1);
  LF_CHILD_OUTPUT_CONNECTIONS(sensor, out, 1, 1, 1);
  LF_CHILD_OUTPUT_EFFECTS(sensor, out, 1, 1, 0);
  LF_CHILD_OUTPUT_OBSERVERS(sensor, out, 1, 1, 0);
  LF_DEFINE_STARTUP_COORDINATOR(Federate);
  LF_DEFINE_SHUTDOWN_COORDINATOR(Federate);
  LF_DEFINE_CLOCK_SYNC(Federate);
} MainSensor;

LF_REACTOR_CTOR_SIGNATURE(MainSensor) {
  LF_REACTOR_CTOR(MainSensor);
  LF_FEDERATE_CTOR_PREAMBLE();
  LF_DEFINE_CHILD_OUTPUT_ARGS(sensor, out, 1, 1);
  LF_INITIALIZE_CHILD_REACTOR_WITH_PARAMETERS(Sensor, sensor, 1, _sensor_out_args[i]);
  LF_INITIALIZE_FEDERATED_CONNECTION_BUNDLE(Sensor, Fusion);
  lf_connect_federated_output(&self->Sensor_Fusion_bundle.outputs[0]->super, &self->sensor->out[0].super);
  LF_INITIALIZE_STARTUP_COORDINATOR(Federate);
  LF_INITIALIZE_SHUTDOWN_COORDINATOR(Federate);
  LF_INITIALIZE_CLOCK_SYNC(Federate);
}

LF_ENTRY_POINT_FEDERATED(MainSensor, 32, 32, 32, MSEC(550), true, 1, false)
int main() { lf_start(); return 0; }
