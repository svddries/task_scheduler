#include "task_scheduler/process.h"

namespace ts
{

// ----------------------------------------------------------------------------------------------------

Process::Process(const std::string& name) : name_(name), running_(false)
{
}

// ----------------------------------------------------------------------------------------------------

Process::~Process()
{
}

// ----------------------------------------------------------------------------------------------------

void Process::run()
{   
    if (running_)
        return;

    running_ = true;
    thread_ = boost::thread(&Process::runImpl, this);
}

// ----------------------------------------------------------------------------------------------------

void Process::stop()
{
    running_ = false;
}

// ----------------------------------------------------------------------------------------------------

void Process::runImpl()
{
    running_ = true;
    while(running_)
    {
        if (!trigger_inputs_.empty())
        {
            const DataChannelId& id = *trigger_inputs_.begin();
            DataChannel& c = blackboard_->channel(id);

            std::unique_lock<std::mutex> lk(c.mutex());
            c.condition_variable().wait(lk);
        }

        if (!running_)
            break;

        process();
    }
}

} // end namespace ts

