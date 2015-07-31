#ifndef TASK_SCHEDULER_BLACKBOARD_H_
#define TASK_SCHEDULER_BLACKBOARD_H_

#include "task_scheduler/data_channel.h"
#include "task_scheduler/types.h"

#include <vector>
#include <map>

namespace ts
{

class Blackboard
{

public:

    Blackboard();

    ~Blackboard();

    DataChannelId RegisterChannel(const std::string& name)
    {
        auto it = name_to_channel_.find(name);
        if (it != name_to_channel_.end())
            return it->second;

        DataChannelId& id = name_to_channel_[name];
        id.idx = channels_.size();
        id.name = name;

        channels_.push_back(new DataChannel);
        return id;
    }

    DataChannel& channel(const DataChannelId& id)
    {
        return *channels_[id.idx];
    }

    void PrintContent();

private:

    std::map<std::string, DataChannelId> name_to_channel_;

    std::vector<DataChannel*> channels_;

};

} // end namespace ts

#endif
