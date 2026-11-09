/* Hand-written equivalent of the `Fusion` federate of Validate3.ulf (reactor-uc df7cfea, POSIX, TCP).
   Compile-time parameter: MAXWAIT_MS. Listens on ports 8921 (sensor A) and 8922 (sensor B). */
#include "reactor-uc/platform/posix/tcp_ip_channel.h"
#include "reactor-uc/schedulers/dynamic/scheduler.h"
#include "reactor-uc/reactor-uc.h"
#include <sys/socket.h>
#ifndef MAXWAIT_MS
#define MAXWAIT_MS 30
#endif
#define MAXWAIT MSEC(MAXWAIT_MS)

lf_ret_t deserialize_int(void *v, const unsigned char *buf, size_t sz) { memcpy(v, buf, sizeof(int)); return LF_OK; }

LF_DEFINE_REACTION_STRUCT(Fusion, r, 0);
LF_DEFINE_REACTION_STP_VIOLATION_HANDLER(Fusion, r);
LF_DEFINE_REACTION_CTOR(Fusion, r, 0, NULL, LF_REACTION_TYPE(Fusion, r_stp_violation_handler));
LF_DEFINE_REACTION_STRUCT(Fusion, s, 0);
LF_DEFINE_REACTION_CTOR(Fusion, s, 1, NULL, NULL);
LF_DEFINE_SHUTDOWN_STRUCT(Fusion, 1, 0);
LF_DEFINE_SHUTDOWN_CTOR(Fusion);
LF_DEFINE_INPUT_STRUCT(Fusion, a, 1, 0, int, 0);
LF_DEFINE_INPUT_CTOR(Fusion, a, 1, 0, int, 0);
LF_DEFINE_INPUT_STRUCT(Fusion, b, 1, 0, int, 0);
LF_DEFINE_INPUT_CTOR(Fusion, b, 1, 0, int, 0);

typedef struct {
  Reactor super;
  LF_REACTION_INSTANCE(Fusion, r);
  LF_REACTION_INSTANCE(Fusion, s);
  LF_SHUTDOWN_INSTANCE(Fusion);
  LF_PORT_INSTANCE(Fusion, a, 1);
  LF_PORT_INSTANCE(Fusion, b, 1);
  int count, tardy, sum_a, sum_b;
  LF_REACTOR_BOOKKEEPING_INSTANCES(2, 3, 0);
} Fusion;

static void line(Environment *env, Fusion *self, const char *what) {
  Fusion_a *a = &self->a[0]; Fusion_b *b = &self->b[0];
  tag_t now = env->scheduler->current_tag(env->scheduler);
  printf("fusion: %s tag (%lld ms, %u), a %s, b %s, lag %lld us\n", what,
         (long long)(env->get_elapsed_logical_time(env) / MSEC(1)), (unsigned)now.microstep,
         lf_is_present(a) ? "present" : "absent ", lf_is_present(b) ? "present" : "absent ",
         (long long)((env->get_physical_time(env) - env->get_logical_time(env)) / USEC(1)));
}

LF_DEFINE_REACTION_BODY(Fusion, r) {
  LF_SCOPE_SELF(Fusion);
  LF_SCOPE_ENV();
  LF_SCOPE_PORT(Fusion, a);
  LF_SCOPE_PORT(Fusion, b);
  self->count++;
  if (lf_is_present(a)) self->sum_a += a->value;
  if (lf_is_present(b)) self->sum_b += b->value;
  line(env, self, "");
}

LF_DEFINE_REACTION_STP_VIOLATION_HANDLER(Fusion, r) {
  LF_SCOPE_SELF(Fusion);
  LF_SCOPE_ENV();
  LF_SCOPE_PORT(Fusion, a);
  LF_SCOPE_PORT(Fusion, b);
  self->tardy++;
  if (lf_is_present(a)) self->sum_a += a->value;
  if (lf_is_present(b)) self->sum_b += b->value;
  line(env, self, "TARDY");
}

LF_DEFINE_REACTION_BODY(Fusion, s) {
  LF_SCOPE_SELF(Fusion);
  int ok = self->count == 8 && self->sum_a == 21 && self->sum_b == 10;
  printf("fusion: %d reactions, %d tardy, sum a %d, sum b %d: %s\n", self->count, self->tardy, self->sum_a,
         self->sum_b, ok ? "PASS" : "FAIL");
}

LF_REACTOR_CTOR_SIGNATURE_WITH_PARAMETERS(Fusion, InputExternalCtorArgs *a_external, InputExternalCtorArgs *b_external) {
  LF_REACTOR_CTOR_PREAMBLE();
  LF_REACTOR_CTOR(Fusion);
  LF_INITIALIZE_REACTION(Fusion, r, NEVER);
  LF_INITIALIZE_REACTION(Fusion, s, NEVER);
  LF_INITIALIZE_SHUTDOWN(Fusion);
  LF_INITIALIZE_INPUT(Fusion, a, 1, a_external);
  LF_INITIALIZE_INPUT(Fusion, b, 1, b_external);
  LF_PORT_REGISTER_EFFECT(self->a, self->r, 1);
  LF_PORT_REGISTER_EFFECT(self->b, self->r, 1);
  LF_SHUTDOWN_REGISTER_EFFECT(self->s);
  self->count = self->tardy = self->sum_a = self->sum_b = 0;
}

LF_DEFINE_FEDERATED_INPUT_CONNECTION_STRUCT(Fusion, a, int, 5);
LF_DEFINE_FEDERATED_INPUT_CONNECTION_CTOR(Fusion, a, int, 5, NEVER, false, MAXWAIT);
LF_DEFINE_FEDERATED_INPUT_CONNECTION_STRUCT(Fusion, b, int, 5);
LF_DEFINE_FEDERATED_INPUT_CONNECTION_CTOR(Fusion, b, int, 5, NEVER, false, MAXWAIT);

typedef struct {
  FederatedConnectionBundle super;
  TcpIpChannel channel;
  LF_FEDERATED_INPUT_CONNECTION_INSTANCE(Fusion, a);
  LF_FEDERATED_CONNECTION_BUNDLE_BOOKKEEPING_INSTANCES(1, 0)
} LF_FEDERATED_CONNECTION_BUNDLE_TYPE(Fusion, SA);

LF_FEDERATED_CONNECTION_BUNDLE_CTOR_SIGNATURE(Fusion, SA) {
  LF_FEDERATED_CONNECTION_BUNDLE_CTOR_PREAMBLE();
  TcpIpChannel_ctor(&self->channel, "127.0.0.1", 8921, AF_INET, true);
  LF_FEDERATED_CONNECTION_BUNDLE_CALL_CTOR();
  LF_INITIALIZE_FEDERATED_INPUT_CONNECTION(Fusion, a, deserialize_int);
}

typedef struct {
  FederatedConnectionBundle super;
  TcpIpChannel channel;
  LF_FEDERATED_INPUT_CONNECTION_INSTANCE(Fusion, b);
  LF_FEDERATED_CONNECTION_BUNDLE_BOOKKEEPING_INSTANCES(1, 0)
} LF_FEDERATED_CONNECTION_BUNDLE_TYPE(Fusion, SB);

LF_FEDERATED_CONNECTION_BUNDLE_CTOR_SIGNATURE(Fusion, SB) {
  LF_FEDERATED_CONNECTION_BUNDLE_CTOR_PREAMBLE();
  TcpIpChannel_ctor(&self->channel, "127.0.0.1", 8922, AF_INET, true);
  LF_FEDERATED_CONNECTION_BUNDLE_CALL_CTOR();
  LF_INITIALIZE_FEDERATED_INPUT_CONNECTION(Fusion, b, deserialize_int);
}

LF_DEFINE_STARTUP_COORDINATOR_STRUCT(Federate, 2, 8);
LF_DEFINE_STARTUP_COORDINATOR_CTOR(Federate, 2, 2, 8, JOIN_IMMEDIATELY);
LF_DEFINE_SHUTDOWN_COORDINATOR_STRUCT(Federate, 8);
LF_DEFINE_SHUTDOWN_COORDINATOR_CTOR(Federate, 2, 8);
LF_DEFINE_CLOCK_SYNC_STRUCT(Federate, 2, 8);
LF_DEFINE_CLOCK_SYNC_DEFAULTS_CTOR(Federate, 2, 8, false);

typedef struct {
  Reactor super;
  LF_CHILD_REACTOR_INSTANCE(Fusion, fusion, 1);
  LF_FEDERATED_CONNECTION_BUNDLE_INSTANCE(Fusion, SA);
  LF_FEDERATED_CONNECTION_BUNDLE_INSTANCE(Fusion, SB);
  LF_FEDERATE_BOOKKEEPING_INSTANCES(2, 0);
  LF_CHILD_INPUT_SOURCES(fusion, a, 1, 1, 0);
  LF_CHILD_INPUT_SOURCES(fusion, b, 1, 1, 0);
  LF_DEFINE_STARTUP_COORDINATOR(Federate);
  LF_DEFINE_SHUTDOWN_COORDINATOR(Federate);
  LF_DEFINE_CLOCK_SYNC(Federate);
} MainFusion;

LF_REACTOR_CTOR_SIGNATURE(MainFusion) {
  LF_REACTOR_CTOR(MainFusion);
  LF_FEDERATE_CTOR_PREAMBLE();
  LF_DEFINE_CHILD_INPUT_ARGS(fusion, a, 1, 1);
  LF_DEFINE_CHILD_INPUT_ARGS(fusion, b, 1, 1);
  LF_INITIALIZE_CHILD_REACTOR_WITH_PARAMETERS(Fusion, fusion, 1, _fusion_a_args[i], _fusion_b_args[i]);
  LF_INITIALIZE_FEDERATED_CONNECTION_BUNDLE(Fusion, SA);
  LF_INITIALIZE_FEDERATED_CONNECTION_BUNDLE(Fusion, SB);
  LF_INITIALIZE_STARTUP_COORDINATOR(Federate);
  LF_INITIALIZE_SHUTDOWN_COORDINATOR(Federate);
  LF_INITIALIZE_CLOCK_SYNC(Federate);
  lf_connect_federated_input(&self->Fusion_SA_bundle.inputs[0]->super, &self->fusion->a[0].super);
  lf_connect_federated_input(&self->Fusion_SB_bundle.inputs[0]->super, &self->fusion->b[0].super);
}

LF_ENTRY_POINT_FEDERATED(MainFusion, 32, 32, 32, MSEC(550), true, 2, false)
int main() { lf_start(); return 0; }
