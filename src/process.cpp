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
        if (!trigger_channels_.empty())
        {
            Time t_data;
            for(unsigned int i = 0; i < trigger_channels_.size(); ++i)
            {
                DataChannel* c = trigger_channels_[i];
                Time t_last_val_ = c->latestTimestamp();

                if (t_last_val_ <= trigger_channel_stamps_[i])
                {
                    // A new value is not yet available, so wait
                    std::unique_lock<std::mutex> lk(c->mutex());
                    c->condition_variable().wait(lk);

                    trigger_channel_stamps_[i] = c->latestTimestamp();
                }
                else
                {
                    trigger_channel_stamps_[i] = t_last_val_;
                }

                t_data = std::max(t_data, trigger_channel_stamps_[i]);

                if (!running_)
                    break;
            }

//            std::cout << name() << ": " << std::endl;

            // Now make sure all input channels are up-to-date
            for(auto& id : inputs_)
            {
                DataChannel& c = blackboard_->channel(id);
                c.waitUntilUpToDate(t_data, name());
            }
        }

        if (!running_)
            break;

        process();
    }
}

} // end namespace ts

