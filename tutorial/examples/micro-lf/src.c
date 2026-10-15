/* Hand-written equivalent of federate `src` in Tardy.ulf (reactor-uc df7cfea, POSIX, TCP). */
#include "reactor-uc/platform/posix/tcp_ip_channel.h"
#include "reactor-uc/schedulers/dynamic/scheduler.h"
#include "reactor-uc/reactor-uc.h"
#include <sys/socket.h>
#define PORT_NUM 8911

LF_DEFINE_TIMER_STRUCT(Src, t, 1, 0)
LF_DEFINE_TIMER_CTOR(Src, t, 1, 0)
LF_DEFINE_REACTION_STRUCT(Src, r, 1)
LF_DEFINE_REACTION_CTOR(Src, r, 0, NULL, NULL)
LF_DEFINE_OUTPUT_STRUCT(Src, out, 1, int)
LF_DEFINE_OUTPUT_CTOR(Src, out, 1)

typedef struct {
  Reactor super;
  LF_TIMER_INSTANCE(Src, t);
  LF_REACTION_INSTANCE(Src, r);
  LF_PORT_INSTANCE(Src, out, 1);
  int n;
  LF_REACTOR_BOOKKEEPING_INSTANCES(1, 2, 0);
} Src;

LF_DEFINE_REACTION_BODY(Src, r) {
  LF_SCOPE_SELF(Src);
  LF_SCOPE_ENV();
  LF_SCOPE_PORT(Src, out);
  self->n++;
  if (self->n == 3) {
    /* Simulate a slow computation: hold the message back 100 ms of physical time. */
    env->platform->wait_for(env->platform, MSEC(100));
  }
#ifdef SPARSE
  if (self->n % 2 == 0) return; /* send only on odd counts */
#endif
  lf_set(out, self->n);
  printf("src: sent %d at logical %lld ms\n", self->n,
         (long long)(env->get_elapsed_logical_time(env) / MSEC(1)));
}

LF_REACTOR_CTOR_SIGNATURE_WITH_PARAMETERS(Src, OutputExternalCtorArgs *out_external) {
  LF_REACTOR_CTOR_PREAMBLE();
  LF_REACTOR_CTOR(Src);
  LF_INITIALIZE_REACTION(Src, r, NEVER);
  LF_INITIALIZE_TIMER(Src, t, MSEC(0), MSEC(200));
  LF_INITIALIZE_OUTPUT(Src, out, 1, out_external);
  LF_TIMER_REGISTER_EFFECT(self->t, self->r);
  LF_PORT_REGISTER_SOURCE(self->out, self->r, 1);
  self->n = 0;
}

int serialize_int(const void *v, size_t sz, unsigned char *buf) { memcpy(buf, v, sizeof(int)); return sizeof(int); }

LF_DEFINE_FEDERATED_OUTPUT_CONNECTION_STRUCT(Src, out, int)
LF_DEFINE_FEDERATED_OUTPUT_CONNECTION_CTOR(Src, out, int, 0)

typedef struct {
  FederatedConnectionBundle super;
  TcpIpChannel channel;
  LF_FEDERATED_OUTPUT_CONNECTION_INSTANCE(Src, out);
  LF_FEDERATED_CONNECTION_BUNDLE_BOOKKEEPING_INSTANCES(0, 1);
} LF_FEDERATED_CONNECTION_BUNDLE_TYPE(Src, Dst);

LF_FEDERATED_CONNECTION_BUNDLE_CTOR_SIGNATURE(Src, Dst) {
  LF_FEDERATED_CONNECTION_BUNDLE_CTOR_PREAMBLE();
  TcpIpChannel_ctor(&self->channel, "127.0.0.1", PORT_NUM, AF_INET, true);
  LF_FEDERATED_CONNECTION_BUNDLE_CALL_CTOR();
  LF_INITIALIZE_FEDERATED_OUTPUT_CONNECTION(Src, out, serialize_int);
}

LF_DEFINE_STARTUP_COORDINATOR_STRUCT(Federate, 1, 6);
LF_DEFINE_STARTUP_COORDINATOR_CTOR(Federate, 1, 1, 6, JOIN_IMMEDIATELY);
LF_DEFINE_SHUTDOWN_COORDINATOR_STRUCT(Federate, 6);
LF_DEFINE_SHUTDOWN_COORDINATOR_CTOR(Federate, 1, 6);
LF_DEFINE_CLOCK_SYNC_STRUCT(Federate, 1, 8);
LF_DEFINE_CLOCK_SYNC_DEFAULTS_CTOR(Federate, 1, 8, true);

typedef struct {
  Reactor super;
  LF_CHILD_REACTOR_INSTANCE(Src, src, 1);
  LF_FEDERATED_CONNECTION_BUNDLE_INSTANCE(Src, Dst);
  LF_FEDERATE_BOOKKEEPING_INSTANCES(1, 1);
  LF_CHILD_OUTPUT_CONNECTIONS(src, out, 1, 1, 1);
  LF_CHILD_OUTPUT_EFFECTS(src, out, 1, 1, 0);
  LF_CHILD_OUTPUT_OBSERVERS(src, out, 1, 1, 0);
  LF_DEFINE_STARTUP_COORDINATOR(Federate);
  LF_DEFINE_SHUTDOWN_COORDINATOR(Federate);
  LF_DEFINE_CLOCK_SYNC(Federate);
} MainSrc;

LF_REACTOR_CTOR_SIGNATURE(MainSrc) {
  LF_REACTOR_CTOR(MainSrc);
  LF_FEDERATE_CTOR_PREAMBLE();
  LF_DEFINE_CHILD_OUTPUT_ARGS(src, out, 1, 1);
  LF_INITIALIZE_CHILD_REACTOR_WITH_PARAMETERS(Src, src, 1, _src_out_args[i]);
  LF_INITIALIZE_FEDERATED_CONNECTION_BUNDLE(Src, Dst);
  lf_connect_federated_output(&self->Src_Dst_bundle.outputs[0]->super, &self->src->out[0].super);
  LF_INITIALIZE_STARTUP_COORDINATOR(Federate);
  LF_INITIALIZE_SHUTDOWN_COORDINATOR(Federate);
  LF_INITIALIZE_CLOCK_SYNC(Federate);
}

LF_ENTRY_POINT_FEDERATED(MainSrc, 32, 32, 32, MSEC(900), true, 1, true)
int main() { lf_start(); return 0; }
