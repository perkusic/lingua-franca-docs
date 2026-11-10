#include "reactor-uc/reactor-uc.h"
#include "reactor-uc/schedulers/dynamic/scheduler.h" 
#include "lf_federate.h"
static Validate3_fusion main_reactor;
static FederatedEnvironment lf_environment;
Environment *_lf_environment = &lf_environment.super;
// Define queues used by scheduler
LF_DEFINE_EVENT_QUEUE(Main_EventQueue, 12)
LF_DEFINE_EVENT_QUEUE(Main_SystemEventQueue, 25)
LF_DEFINE_REACTION_QUEUE(Main_ReactionQueue, 2)
static DynamicScheduler _scheduler;
static Scheduler* scheduler = &_scheduler.super;
void lf_exit(void) {
   FederatedEnvironment_free(&lf_environment);
}
void lf_start(void) {
    // Define queues used by scheduler
    LF_INITIALIZE_EVENT_QUEUE(Main_EventQueue, 12)
    LF_INITIALIZE_EVENT_QUEUE(Main_SystemEventQueue, 25)
    LF_INITIALIZE_REACTION_QUEUE(Main_ReactionQueue, 2)
    DynamicScheduler_ctor(&_scheduler, _lf_environment, &Main_EventQueue.super, &Main_SystemEventQueue.super, &Main_ReactionQueue.super, MSEC(550), true);
    FederatedEnvironment_ctor(&lf_environment, (Reactor *)&main_reactor, scheduler, false,  
                     (FederatedConnectionBundle **) &main_reactor._bundles, 2, &main_reactor.startup_coordinator.super, 
                     &main_reactor.shutdown_coordinator.super, NULL);
    Validate3_fusion_ctor(&main_reactor, NULL, _lf_environment);
    _lf_environment->assemble(_lf_environment);
    _lf_environment->start(_lf_environment);
    lf_exit();
}
