#ifndef TASK_SCHEDULER_TYPES_H_
#define TASK_SCHEDULER_TYPES_H_

#include <string>

namespace ts
{

struct DataChannelId
{
    unsigned long idx;
    std::string name;

    bool operator<(const DataChannelId& other) const
    {
        return idx < other.idx;
    }

    bool operator==(const DataChannelId& other) const
    {
        return idx == other.idx;
    }

};

class Process;
class Blackboard;

} // end namespace ts

#endif
