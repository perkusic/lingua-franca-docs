#include "Validate3/Sensor.h"
// Private preambles

// Reaction bodies
LF_DEFINE_REACTION_BODY(Reactor_Sensor, reaction0) {
  // Bring self struct, environment, triggers, effects and sources into scope.
    LF_SCOPE_SELF(Reactor_Sensor);
    LF_SCOPE_ENV();
    LF_SCOPE_TIMER(Reactor_Sensor, t);
    LF_SCOPE_PORT(Reactor_Sensor, out);
  // Start of user-witten reaction body
  self->n++;
  if (self->n == self->slow_at) {
    env->platform->wait_for(env->platform, self->slow_by);  // a slow computation
  }
  lf_set(out, self->n);
}
// Reaction deadline violation handlers

// Reaction STP violation handlers

// Reaction constructors



LF_DEFINE_REACTION_CTOR(Reactor_Sensor, reaction0, 0, NULL, NULL);
        
// Timer constructors 
LF_DEFINE_TIMER_CTOR(Reactor_Sensor, t, 1, 0);
// Port constructors
LF_DEFINE_OUTPUT_CTOR(Reactor_Sensor, out, 1);
// Connection constructors

LF_REACTOR_CTOR_SIGNATURE_WITH_PARAMETERS(Reactor_Sensor , OutputExternalCtorArgs *_out_external , interval_t period, int slow_at, interval_t slow_by ) {
   LF_REACTOR_CTOR_PREAMBLE();
   LF_REACTOR_CTOR(Reactor_Sensor);
   // Initialize Reactor parameters
   
   self->period = period;
   
   self->slow_at = slow_at;
   
   self->slow_by = slow_by;
   // Initialize State variables 
   self->n = 0;
        
   // Initialize Timers
   LF_INITIALIZE_TIMER(Reactor_Sensor, t, 0, period);
   // Initialize actions and builtin triggers
   
   // Initialize ports
   LF_INITIALIZE_OUTPUT(Reactor_Sensor, out, 1, _out_external);
   // Initialize connections
   
   // Do connections 
   
   // Initialize Reactions 
   LF_INITIALIZE_REACTION(Reactor_Sensor, reaction0, NEVER); 
     LF_TIMER_REGISTER_EFFECT(self->t, self->reaction0);
     LF_PORT_REGISTER_SOURCE(self->out, self->reaction0, 1);
}
