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

    InputPort() : blackboard_(0) {}

    InputPort(Blackboard* bb, const DataChannelId& channel_id) : blackboard_(bb), channel_id_(channel_id) {}

    bool read(double& v)
    {
        if (!blackboard_)
        {
//            std::cout << "Input port was not initialized" << std::endl;
            return false;
        }

        return blackboard_->channel(channel_id_).read(v);
    }

private:

    Blackboard* blackboard_;

    DataChannelId channel_id_;

};

// ----------------------------------------------------------------------------------------------------

class OutputPort
{

public:

    OutputPort() : blackboard_(0) {}

    OutputPort(Blackboard* bb, const DataChannelId& channel_id) : blackboard_(bb), channel_id_(channel_id) {}

    void write(const double& v)
    {
        if (!blackboard_)
        {
//            std::cout << "Output port was not initialized" << std::endl;
            return;
        }

        return blackboard_->channel(channel_id_).write(v);
    }

private:

    Blackboard* blackboard_;

    DataChannelId channel_id_;
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
            trigger_inputs_.insert(id);

        port = InputPort(blackboard_, id);
    }

    void RegisterOutput(const std::string& name, OutputPort& port)
    {
        DataChannelId id = blackboard_->RegisterChannel(name);
        inputs_.insert(id);

        port = OutputPort(blackboard_, id);
    }

    void setBlackboard(Blackboard* blackboard)
    {
        blackboard_ = blackboard;
    }

    const std::set<DataChannelId>& trigger_inputs() const { return trigger_inputs_; }

    const std::set<DataChannelId>& inputs() const { return inputs_; }

    const std::set<DataChannelId>& outputs() const { return outputs_; }

    const std::string& name() const { return name_; }

private:

    std::string name_;

    Blackboard* blackboard_;

    std::set<DataChannelId> trigger_inputs_;

    std::set<DataChannelId> inputs_;

    std::set<DataChannelId> outputs_;


    // Threading

    boost::thread thread_;

    bool running_;

};

} // end namespace ts

#endif
