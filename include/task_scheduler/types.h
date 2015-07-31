#ifndef TASK_SCHEDULER_TYPES_H_
#define TASK_SCHEDULER_TYPES_H_

#include <string>
#include <chrono>
#include <iostream>

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

typedef std::chrono::system_clock::time_point Time;
typedef std::chrono::system_clock::duration Duration;

struct PrintDuration
{
    PrintDuration(const Duration& dur) : dur_(dur) {}

    friend inline std::ostream& operator<<(std::ostream& out, const PrintDuration& p)
    {
        out << std::chrono::duration_cast<std::chrono::duration<float>>(p.dur_).count() << " sec";
        return out;
    }

    const Duration& dur_;
};

struct PrintTime
{
    PrintTime(const Time& time) : time_(time) {}

    friend inline std::ostream& operator<<(std::ostream& out, const PrintTime& t)
    {
        out << std::chrono::duration_cast<std::chrono::duration<float>>(t.time_.time_since_epoch()).count() << "s epoch";
        return out;
    }

    const Time& time_;
};

class Process;
class Blackboard;

} // end namespace ts

#endif
