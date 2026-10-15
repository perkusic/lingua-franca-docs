/* Hand-written equivalent of federate `dst` in Tardy.ulf (reactor-uc df7cfea, POSIX, TCP). */
#include "reactor-uc/platform/posix/tcp_ip_channel.h"
#include "reactor-uc/schedulers/dynamic/scheduler.h"
#include "reactor-uc/reactor-uc.h"
#include <sys/socket.h>
#define PORT_NUM 8911
#ifndef MAXWAIT_MS
#define MAXWAIT_MS 20
#endif
#define MAXWAIT MSEC(MAXWAIT_MS)

lf_ret_t deserialize_int(void *v, const unsigned char *buf, size_t sz) { memcpy(v, buf, sizeof(int)); return LF_OK; }

LF_DEFINE_TIMER_STRUCT(Dst, t, 1, 0)
LF_DEFINE_TIMER_CTOR(Dst, t, 1, 0)
LF_DEFINE_REACTION_STRUCT(Dst, r, 0);
LF_DEFINE_REACTION_STP_VIOLATION_HANDLER(Dst, r);
#ifdef NO_HANDLER
LF_DEFINE_REACTION_CTOR(Dst, r, 0, NULL, NULL);
#else
LF_DEFINE_REACTION_CTOR(Dst, r, 0, NULL, LF_REACTION_TYPE(Dst, r_stp_violation_handler));
#endif
LF_DEFINE_INPUT_STRUCT(Dst, in, 1, 0, int, 0);
LF_DEFINE_INPUT_CTOR(Dst, in, 1, 0, int, 0);

typedef struct {
  Reactor super;
  LF_TIMER_INSTANCE(Dst, t);
  LF_REACTION_INSTANCE(Dst, r);
  LF_PORT_INSTANCE(Dst, in, 1);
  LF_REACTOR_BOOKKEEPING_INSTANCES(1, 2, 0);
} Dst;

LF_DEFINE_REACTION_BODY(Dst, r) {
  LF_SCOPE_SELF(Dst);
  LF_SCOPE_ENV();
  LF_SCOPE_PORT(Dst, in);
  tag_t now = env->scheduler->current_tag(env->scheduler);
  long long ms = (long long)(env->get_elapsed_logical_time(env) / MSEC(1));
  long long lag = (long long)((env->get_physical_time(env) - env->get_logical_time(env)) / MSEC(1));
  if (lf_is_present(in)) printf("dst: tag (%lld ms, %u), lag %lld ms: in = %d\n", ms, (unsigned)now.microstep, lag, in->value);
  else printf("dst: tag (%lld ms, %u), lag %lld ms: in absent\n", ms, (unsigned)now.microstep, lag);
}

LF_DEFINE_REACTION_STP_VIOLATION_HANDLER(Dst, r) {
  LF_SCOPE_SELF(Dst);
  LF_SCOPE_ENV();
  LF_SCOPE_PORT(Dst, in);
  tag_t now = env->scheduler->current_tag(env->scheduler);
  printf("dst: TARDY in = %d, intended tag %lld ms, handled at tag (%lld ms, %u)\n", in->value,
         (long long)((in->super.intended_tag.time - env->scheduler->start_time) / MSEC(1)),
         (long long)((now.time - env->scheduler->start_time) / MSEC(1)), (unsigned)now.microstep);
}

LF_REACTOR_CTOR_SIGNATURE_WITH_PARAMETERS(Dst, InputExternalCtorArgs *in_external) {
  LF_REACTOR_CTOR_PREAMBLE();
  LF_REACTOR_CTOR(Dst);
  LF_INITIALIZE_REACTION(Dst, r, NEVER);
  LF_INITIALIZE_TIMER(Dst, t, MSEC(0), MSEC(200));
  LF_INITIALIZE_INPUT(Dst, in, 1, in_external);
  LF_TIMER_REGISTER_EFFECT(self->t, self->r);
  LF_PORT_REGISTER_EFFECT(self->in, self->r, 1);
}

LF_DEFINE_FEDERATED_INPUT_CONNECTION_STRUCT(Dst, in, int, 5);
LF_DEFINE_FEDERATED_INPUT_CONNECTION_CTOR(Dst, in, int, 5, NEVER, false, MAXWAIT);

typedef struct {
  FederatedConnectionBundle super;
  TcpIpChannel channel;
  LF_FEDERATED_INPUT_CONNECTION_INSTANCE(Dst, in);
  LF_FEDERATED_CONNECTION_BUNDLE_BOOKKEEPING_INSTANCES(1, 0)
} LF_FEDERATED_CONNECTION_BUNDLE_TYPE(Dst, Src);

LF_FEDERATED_CONNECTION_BUNDLE_CTOR_SIGNATURE(Dst, Src) {
  LF_FEDERATED_CONNECTION_BUNDLE_CTOR_PREAMBLE();
  TcpIpChannel_ctor(&self->channel, "127.0.0.1", PORT_NUM, AF_INET, false);
  LF_FEDERATED_CONNECTION_BUNDLE_CALL_CTOR();
  LF_INITIALIZE_FEDERATED_INPUT_CONNECTION(Dst, in, deserialize_int);
}

LF_DEFINE_STARTUP_COORDINATOR_STRUCT(Federate, 1, 6);
LF_DEFINE_SHUTDOWN_COORDINATOR_STRUCT(Federate, 6);
LF_DEFINE_STARTUP_COORDINATOR_CTOR(Federate, 1, 1, 6, JOIN_IMMEDIATELY);
LF_DEFINE_SHUTDOWN_COORDINATOR_CTOR(Federate, 1, 6);
LF_DEFINE_CLOCK_SYNC_STRUCT(Federate, 1, 8);
LF_DEFINE_CLOCK_SYNC_DEFAULTS_CTOR(Federate, 1, 8, false);

typedef struct {
  Reactor super;
  LF_CHILD_REACTOR_INSTANCE(Dst, dst, 1);
  LF_FEDERATED_CONNECTION_BUNDLE_INSTANCE(Dst, Src);
  LF_FEDERATE_BOOKKEEPING_INSTANCES(1, 0);
  LF_CHILD_INPUT_SOURCES(dst, in, 1, 1, 0);
  LF_DEFINE_STARTUP_COORDINATOR(Federate);
  LF_DEFINE_SHUTDOWN_COORDINATOR(Federate);
  LF_DEFINE_CLOCK_SYNC(Federate);
} MainDst;

LF_REACTOR_CTOR_SIGNATURE(MainDst) {
  LF_REACTOR_CTOR(MainDst);
  LF_FEDERATE_CTOR_PREAMBLE();
  LF_DEFINE_CHILD_INPUT_ARGS(dst, in, 1, 1);
  LF_INITIALIZE_CHILD_REACTOR_WITH_PARAMETERS(Dst, dst, 1, _dst_in_args[i]);
  LF_INITIALIZE_FEDERATED_CONNECTION_BUNDLE(Dst, Src);
  LF_INITIALIZE_STARTUP_COORDINATOR(Federate);
  LF_INITIALIZE_SHUTDOWN_COORDINATOR(Federate);
  LF_INITIALIZE_CLOCK_SYNC(Federate);
  lf_connect_federated_input(&self->Dst_Src_bundle.inputs[0]->super, &self->dst->in[0].super);
}

LF_ENTRY_POINT_FEDERATED(MainDst, 32, 32, 32, MSEC(900), true, 1, true)
int main() { lf_start(); return 0; }
