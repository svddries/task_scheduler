#ifndef TASK_SCHEDULER_DATA_CHANNEL_H_
#define TASK_SCHEDULER_DATA_CHANNEL_H_

#include "task_scheduler/types.h"

#include <vector>
#include <condition_variable>

#include <iostream>

namespace ts
{

class DataChannel
{

public:

    DataChannel() : t_latest_(0) {}

    ~DataChannel() {}

    bool read(double& v, Time& timestamp)
    {
        if (values_.empty())
            return false;

        v = values_.back();
        timestamp = t_latest_;
        return true;
    }

    void write(const double& v, const Time& timestamp)
    {
        if (timestamp < t_latest_)
        {
            std::cout << "Received earlier stamp!" << std::endl;
            return;
        }

        values_.clear();
        values_.push_back(v);
        t_latest_ = timestamp;

        // Notify threads that are waiting for a new value
        cv_.notify_all();
    }

    Time latestTimestamp() const { return t_latest_; }

    std::mutex& mutex() { return m_; }
    std::condition_variable& condition_variable() { return cv_; }

private:

    Time t_latest_;

    std::vector<double> values_;

    std::mutex m_;

    std::condition_variable cv_;

};

} // end namespace ts

#endif
