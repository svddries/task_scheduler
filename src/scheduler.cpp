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
    }

    for(Process* p : processes_)
    {
        p->run();
    }
}

} // end namespace ts

