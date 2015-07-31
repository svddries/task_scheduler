#ifndef TASK_SCHEDULER_PROCESS_H_
#define TASK_SCHEDULER_PROCESS_H_

#include "task_scheduler/types.h"
#include "task_scheduler/blackboard.h"

#include <set>
//#include <iostream>

// Threading
#include <boost/thread.hpp>
#include <mutex>
#include <condition_variable>

namespace ts
{

// ----------------------------------------------------------------------------------------------------

class InputPort
{

public:

    InputPort() : channel_(0) {}

    InputPort(DataChannel* channel) : channel_(channel) {}

    bool read(double& v, Time& timestamp)
    {
        if (!channel_)
        {
//            std::cout << "Input port was not initialized" << std::endl;
            return false;
        }

        return channel_->read(v, timestamp);
    }

private:

    DataChannel* channel_;

};

// ----------------------------------------------------------------------------------------------------

class OutputPort
{

public:

    OutputPort() : channel_(0) {}

    OutputPort(DataChannel* channel, const std::string& process_name) : channel_(channel), process_name_(process_name) {}

    void write(const double& v, const Time& timestamp)
    {
        if (!channel_)
        {
//            std::cout << "Output port was not initialized" << std::endl;
            return;
        }

        return channel_->write(v, timestamp, process_name_);
    }

private:

    DataChannel* channel_;

    std::string process_name_;
};

// ----------------------------------------------------------------------------------------------------

class Process
{

public:

    Process(const std::string& name);

    ~Process();

    virtual void initialize() {}

    virtual void process() {}

    void run();

    void runImpl();


    void stop();

    void RegisterInput(const std::string& name, InputPort& port, bool is_trigger)
    {
        DataChannelId id = blackboard_->RegisterChannel(name);
        inputs_.insert(id);

        if (is_trigger)
        {
            trigger_channels_.push_back(&blackboard_->channel(id));
            trigger_channel_stamps_.push_back(Time());
        }

        port = InputPort(&blackboard_->channel(id));
    }

    void RegisterOutput(const std::string& name, OutputPort& port)
    {
        DataChannelId id = blackboard_->RegisterChannel(name);
        outputs_.insert(id);

        port = OutputPort(&blackboard_->channel(id), name_);
    }

    void setBlackboard(Blackboard* blackboard)
    {
        blackboard_ = blackboard;
    }

    const std::set<DataChannelId>& inputs() const { return inputs_; }

    const std::set<DataChannelId>& outputs() const { return outputs_; }

    const std::string& name() const { return name_; }

private:

    std::string name_;

    Blackboard* blackboard_;

    std::vector<DataChannel*> trigger_channels_;
    std::vector<Time> trigger_channel_stamps_;

    std::set<DataChannelId> inputs_;

    std::set<DataChannelId> outputs_;


    // Threading

    boost::thread thread_;

    bool running_;

};

} // end namespace ts

#endif
