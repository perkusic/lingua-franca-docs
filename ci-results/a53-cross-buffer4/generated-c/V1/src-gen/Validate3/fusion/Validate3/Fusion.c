#include "Validate3/Fusion.h"
// Private preambles

// Reaction bodies
LF_DEFINE_REACTION_BODY(Reactor_Fusion, reaction0) {
  // Bring self struct, environment, triggers, effects and sources into scope.
    LF_SCOPE_SELF(Reactor_Fusion);
    LF_SCOPE_ENV();
    LF_SCOPE_PORT(Reactor_Fusion, a);
    LF_SCOPE_PORT(Reactor_Fusion, b);
  // Start of user-witten reaction body
  self->count++;
  if (lf_is_present(a)) self->sum_a += a->value;
  if (lf_is_present(b)) self->sum_b += b->value;
  tag_t now = env->scheduler->current_tag(env->scheduler);
  printf("fusion: tag (%lld ms, %u), a %s, b %s, lag %lld us\n",
         (long long)(env->get_elapsed_logical_time(env) / MSEC(1)),
         (unsigned)now.microstep,
         lf_is_present(a) ? "present" : "absent",
         lf_is_present(b) ? "present" : "absent",
         (long long)((env->get_physical_time(env) - env->get_logical_time(env)) / USEC(1)));
}
LF_DEFINE_REACTION_BODY(Reactor_Fusion, reaction1) {
  // Bring self struct, environment, triggers, effects and sources into scope.
    LF_SCOPE_SELF(Reactor_Fusion);
    LF_SCOPE_ENV();
    LF_SCOPE_SHUTDOWN(Reactor_Fusion);
  // Start of user-witten reaction body
  int ok = self->count == 8 && self->sum_a == 21 && self->sum_b == 10;
  printf("fusion: %d reactions, %d tardy, sum a %d, sum b %d: %s\n",
         self->count, self->tardy_count, self->sum_a, self->sum_b, ok ? "PASS" : "FAIL");
}
// Reaction deadline violation handlers

// Reaction STP violation handlers
LF_DEFINE_REACTION_STP_VIOLATION_HANDLER(Reactor_Fusion, reaction0) {
  // Bring self struct, environment, triggers, effects and sources into scope.
    LF_SCOPE_SELF(Reactor_Fusion);
    LF_SCOPE_ENV();
    LF_SCOPE_PORT(Reactor_Fusion, a);
    LF_SCOPE_PORT(Reactor_Fusion, b);
  // Start of user-witten reaction STAA violation handler body
  self->tardy_count++;
  if (lf_is_present(a)) self->sum_a += a->value;
  if (lf_is_present(b)) self->sum_b += b->value;
  tag_t now = env->scheduler->current_tag(env->scheduler);
  printf("fusion: TARDY tag (%lld ms, %u), a %s, b %s, lag %lld us\n",
         (long long)(env->get_elapsed_logical_time(env) / MSEC(1)),
         (unsigned)now.microstep,
         lf_is_present(a) ? "present" : "absent",
         lf_is_present(b) ? "present" : "absent",
         (long long)((env->get_physical_time(env) - env->get_logical_time(env)) / USEC(1)));
}
// Reaction constructors

LF_DEFINE_REACTION_STP_VIOLATION_HANDLER(Reactor_Fusion, reaction0);

LF_DEFINE_REACTION_CTOR(Reactor_Fusion, reaction0, 0, NULL, LF_REACTION_TYPE(Reactor_Fusion, reaction0_stp_violation_handler));



LF_DEFINE_REACTION_CTOR(Reactor_Fusion, reaction1, 1, NULL, NULL);
LF_DEFINE_SHUTDOWN_CTOR(Reactor_Fusion);
// Timer constructors 

// Port constructors
LF_DEFINE_INPUT_CTOR(Reactor_Fusion, a, 1, 0, int, 0);
LF_DEFINE_INPUT_CTOR(Reactor_Fusion, b, 1, 0, int, 0);
// Connection constructors

LF_REACTOR_CTOR_SIGNATURE_WITH_PARAMETERS(Reactor_Fusion , InputExternalCtorArgs *_a_external, InputExternalCtorArgs *_b_external  ) {
   LF_REACTOR_CTOR_PREAMBLE();
   LF_REACTOR_CTOR(Reactor_Fusion);
   // Initialize Reactor parameters
   
   // Initialize State variables 
   self->count = 0;
   self->tardy_count = 0;
   self->sum_a = 0;
   self->sum_b = 0;
        
   // Initialize Timers
   // Initialize actions and builtin triggers
   
   LF_INITIALIZE_SHUTDOWN(Reactor_Fusion);
   // Initialize ports
   LF_INITIALIZE_INPUT(Reactor_Fusion, a, 1, _a_external);
   LF_INITIALIZE_INPUT(Reactor_Fusion, b, 1, _b_external);
   // Initialize connections
   
   // Do connections 
   
   // Initialize Reactions 
   LF_INITIALIZE_REACTION(Reactor_Fusion, reaction0, NEVER); 
     LF_PORT_REGISTER_EFFECT(self->a, self->reaction0, 1);
     LF_PORT_REGISTER_EFFECT(self->b, self->reaction0, 1);
           
           
   LF_INITIALIZE_REACTION(Reactor_Fusion, reaction1, NEVER); 
     LF_SHUTDOWN_REGISTER_EFFECT(self->reaction1);
           
}
