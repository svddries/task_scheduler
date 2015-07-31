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

class DataChannel
{

public:

    DataChannel() : max_msg_delay_(Duration::zero()) {}

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

        if (!values_.empty())
        {
            Duration msg_delay = now - timestamp;

            std::cout << "From " << source << ": Message delay: " << PrintDuration(msg_delay) << std::endl;

            max_msg_delay_ = std::max(max_msg_delay_, msg_delay);

            std::cout << "   Max message delay: " << PrintDuration(max_msg_delay_) << std::endl;

            if (timestamp < t_latest_data_)
            {
                std::cout << "Received earlier stamp! " << PrintDuration(t_latest_data_ - timestamp) << " too old" << std::endl;
                return;
            }
        }

        values_.clear();
        values_.push_back(v);
        t_latest_data_ = timestamp;

        // Notify threads that are waiting for a new value
        cv_.notify_all();
    }

    // Returns true if this channel does not expect to receive any updates
    // that will change it's state BEFORE 'time'. In other words: if this returns
    // true, the reader can be assured that it is using data that will not change
    // in the future for the specified point in time.
    bool is_up_to_date(const Time& time) const
    {
        Time now = std::chrono::system_clock::now();
        Duration delay = now - time;
        return delay > max_msg_delay_;
    }

    void waitUntilUpToDate(const Time& time) const
    {
        Time now = std::chrono::system_clock::now();
        Duration delay = now - time;
        if (delay > max_msg_delay_)
        {
            std::cout << "Don't need to wait" << std::endl;
            return;
        }


        Duration wait_time = max_msg_delay_ - delay;
        wait_time *= 1.1;

        std::cout << "Need to wait for " << PrintDuration(wait_time) << std::endl;

        unsigned long wait_time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(wait_time).count();

        boost::this_thread::sleep_for(boost::chrono::nanoseconds(wait_time_ns));
    }

    Time latestTimestamp() const { return t_latest_data_; }

    std::mutex& mutex() { return m_; }
    std::condition_variable& condition_variable() { return cv_; }

private:

    Time t_latest_data_;

    Duration max_msg_delay_;

    std::vector<double> values_;

    std::mutex m_;

    std::condition_variable cv_;

};

} // end namespace ts

#endif
