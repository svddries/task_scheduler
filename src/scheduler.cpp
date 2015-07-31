#include "task_scheduler/scheduler.h"
#include "task_scheduler/process.h"

namespace ts
{

// ----------------------------------------------------------------------------------------------------

Scheduler::Scheduler()
{
}

// ----------------------------------------------------------------------------------------------------

Scheduler::~Scheduler()
{
}

// ----------------------------------------------------------------------------------------------------

void Scheduler::AddProcess(Process* p)
{
    p->setBlackboard(&blackboard_);
    processes_.push_back(p);
}

// ----------------------------------------------------------------------------------------------------

void Scheduler::Run()
{
    // Initialize all processes
    for(Process* p : processes_)
    {
        p->initialize();

        // Register trigger inputs for this process
        for(const DataChannelId& c : p->trigger_inputs())
            triggers_[c] = p;
    }

    for(Process* p : processes_)
    {
        p->run();
    }
}

} // end namespace ts

