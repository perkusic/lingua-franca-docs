#include "lf_federate.h"

LF_DEFINE_STARTUP_COORDINATOR_CTOR(Federate, 2, 2, 9, JOIN_IMMEDIATELY);
LF_DEFINE_SHUTDOWN_COORDINATOR_CTOR(Federate, 2, 8);
        
// Federated Connections
// Federated input and output connection constructors
LF_DEFINE_FEDERATED_INPUT_CONNECTION_CTOR(Reactor_Validate3, conn_sa_out_0, int, 4, NEVER, false, MSEC(65));
LF_FEDERATED_CONNECTION_BUNDLE_CTOR_SIGNATURE(Validate3_sa, Validate3_fusion) {
  LF_FEDERATED_CONNECTION_BUNDLE_CTOR_PREAMBLE();
  TcpIpChannel_ctor(&self->channel, "127.0.0.1", 1024, AF_INET, false);
  LF_FEDERATED_CONNECTION_BUNDLE_CALL_CTOR();
  LF_INITIALIZE_FEDERATED_INPUT_CONNECTION(Reactor_Validate3, conn_sa_out_0, deserialize_payload_default);
}
// Federated input and output connection constructors
LF_DEFINE_FEDERATED_INPUT_CONNECTION_CTOR(Reactor_Validate3, conn_sb_out_1, int, 4, NEVER, false, MSEC(65));
LF_FEDERATED_CONNECTION_BUNDLE_CTOR_SIGNATURE(Validate3_sb, Validate3_fusion) {
  LF_FEDERATED_CONNECTION_BUNDLE_CTOR_PREAMBLE();
  TcpIpChannel_ctor(&self->channel, "127.0.0.1", 1026, AF_INET, false);
  LF_FEDERATED_CONNECTION_BUNDLE_CALL_CTOR();
  LF_INITIALIZE_FEDERATED_INPUT_CONNECTION(Reactor_Validate3, conn_sb_out_1, deserialize_payload_default);
}
// Connection constructors

LF_REACTOR_CTOR_SIGNATURE(Validate3_fusion) {
   LF_FEDERATE_CTOR_PREAMBLE();
   LF_REACTOR_CTOR(Validate3_fusion);
   
   LF_DEFINE_CHILD_INPUT_ARGS(fusion, a, 1, 1);
   LF_DEFINE_CHILD_INPUT_ARGS(fusion, b, 1, 1);
   LF_INITIALIZE_CHILD_REACTOR_WITH_PARAMETERS(Reactor_Fusion, fusion, 1 , _fusion_a_args[i], _fusion_b_args[i] );
   // Initialize connection bundles
   LF_INITIALIZE_FEDERATED_CONNECTION_BUNDLE(Validate3_sa, Validate3_fusion);
   LF_INITIALIZE_FEDERATED_CONNECTION_BUNDLE(Validate3_sb, Validate3_fusion);
   // Do connections 
   lf_connect_federated_input((Connection *)&self->LF_FEDERATED_CONNECTION_BUNDLE_NAME(Validate3_sa, Validate3_fusion).conn_sa_out_0, (Port*) &self->fusion[0].a[0]);
   lf_connect_federated_input((Connection *)&self->LF_FEDERATED_CONNECTION_BUNDLE_NAME(Validate3_sb, Validate3_fusion).conn_sb_out_1, (Port*) &self->fusion[0].b[0]);
   // Initialize connections
   
   // Do connections 
   
        
   LF_INITIALIZE_STARTUP_COORDINATOR(Federate);
   LF_INITIALIZE_SHUTDOWN_COORDINATOR(Federate);
}
