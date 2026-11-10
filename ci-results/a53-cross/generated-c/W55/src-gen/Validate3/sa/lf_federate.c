#include "lf_federate.h"

LF_DEFINE_STARTUP_COORDINATOR_CTOR(Federate, 1, 2, 6, JOIN_IMMEDIATELY);
LF_DEFINE_SHUTDOWN_COORDINATOR_CTOR(Federate, 2, 5);
        
// Federated Connections
// Federated input and output connection constructors
LF_DEFINE_FEDERATED_OUTPUT_CONNECTION_CTOR(Reactor_Validate3, conn_sa_out_0, int, 0);
LF_FEDERATED_CONNECTION_BUNDLE_CTOR_SIGNATURE(Validate3_sa, Validate3_fusion) {
  LF_FEDERATED_CONNECTION_BUNDLE_CTOR_PREAMBLE();
  TcpIpChannel_ctor(&self->channel, "127.0.0.1", 1024, AF_INET, true);
  LF_FEDERATED_CONNECTION_BUNDLE_CALL_CTOR();
  LF_INITIALIZE_FEDERATED_OUTPUT_CONNECTION(Reactor_Validate3, conn_sa_out_0, serialize_payload_default);
}
// Connection constructors

LF_REACTOR_CTOR_SIGNATURE(Validate3_sa) {
   LF_FEDERATE_CTOR_PREAMBLE();
   LF_REACTOR_CTOR(Validate3_sa);
   LF_DEFINE_CHILD_OUTPUT_ARGS(sa, out, 1, 1);
   
   LF_INITIALIZE_CHILD_REACTOR_WITH_PARAMETERS(Reactor_Sensor, sa, 1 , _sa_out_args[i] , MSEC(100), 0, 0);
   // Initialize connection bundles
   LF_INITIALIZE_FEDERATED_CONNECTION_BUNDLE(Validate3_sa, Validate3_fusion);
   // Do connections 
   lf_connect_federated_output((Connection *)&self->LF_FEDERATED_CONNECTION_BUNDLE_NAME(Validate3_sa, Validate3_fusion).conn_sa_out_0, (Port*) &self->sa[0].out[0]);
   // Initialize connections
   
   // Do connections 
   
        
   LF_INITIALIZE_STARTUP_COORDINATOR(Federate);
   LF_INITIALIZE_SHUTDOWN_COORDINATOR(Federate);
}
