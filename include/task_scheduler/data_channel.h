#ifndef TASK_SCHEDULER_DATA_CHANNEL_H_
#define TASK_SCHEDULER_DATA_CHANNEL_H_

#include "task_scheduler/types.h"

#include <vector>
#include <condition_variable>
#include <boost/thread.hpp>

#include <iostream>

namespace ts
{

// ----------------------------------------------------------------------------------------------------

struct SourceStats
{
    SourceStats() : initialized(false), max_msg_delay(Duration::zero()) {}

    bool initialized;
    Time t_last_data;
    Duration max_msg_delay;
    Duration msg_intervals;

    void update(const Time& t_data, const Time& t_arrival)
    {
        if (initialized)
            msg_intervals = t_data - t_last_data;
        else
            initialized = true;

        max_msg_delay = std::max(max_msg_delay, t_arrival - t_data);
        t_last_data = t_data;
    }

    bool expectedNextUpdate(Time& t_data, Time& t_arrival) const
    {
        if (!initialized)
            return false;

        t_data = t_last_data + msg_intervals;
        t_arrival = t_data + max_msg_delay;
    }
};

// ----------------------------------------------------------------------------------------------------

class DataChannel
{

public:

    DataChannel() {}

    ~DataChannel() {}

    bool read(double& v, Time& timestamp)
    {
        if (values_.empty())
            return false;

        v = values_.back();
        timestamp = t_latest_data_;
        return true;
    }

    void write(const double& v, const Time& timestamp, const std::string& source)
    {
        Time now = std::chrono::system_clock::now();

        SourceStats& stats = source_stats_[source];
        stats.update(timestamp, now);

        if (timestamp < t_latest_data_)
        {
            std::cout << "From [" << source << "] Received earlier stamp! " << PrintDuration(t_latest_data_ - timestamp) << " too old" << std::endl;
            return;
        }

        values_.clear();
        values_.push_back(v);
        t_latest_data_ = timestamp;

        // Notify threads that are waiting for a new value
        cv_.notify_all();
    }

    void waitUntilUpToDate(const Time& time, const std::string& source) const
    {
        if (source_stats_.size() <= 1)
            return;

        for(auto it : source_stats_)
        {
            if (it.first == source)
                continue;

            const SourceStats& stats = it.second;

            Time t_data, t_arrival;
            stats.expectedNextUpdate(t_data, t_arrival);

            if (t_data > time)
                continue;

//            std::cout << "    " << source << ": " << it.first << ": " << PrintDuration(time - t_data) << std::endl;

            Time now = std::chrono::system_clock::now();
            if (t_arrival < now)
                continue;

            Duration wait_time = t_arrival - now;

            wait_time *= 1.1; // Add some slack

            unsigned long wait_time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(wait_time).count();
            boost::this_thread::sleep_for(boost::chrono::nanoseconds(wait_time_ns));
        }
    }

    Time latestTimestamp() const { return t_latest_data_; }

    std::mutex& mutex() { return m_; }
    std::condition_variable& condition_variable() { return cv_; }

private:

    Time t_latest_data_;

    std::map<std::string, SourceStats> source_stats_;

    std::vector<double> values_;

    std::mutex m_;

    std::condition_variable cv_;

};

} // end namespace ts

#endif
