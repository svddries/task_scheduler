#ifndef TASK_SCHEDULER_DATA_CHANNEL_H_
#define TASK_SCHEDULER_DATA_CHANNEL_H_

#include <vector>
#include <condition_variable>

namespace ts
{

class DataChannel
{

public:

    DataChannel();

    ~DataChannel();

    bool read(double& v)
    {
        if (values_.empty())
            return false;

        v = values_.back();
        return true;
    }

    void write(const double& v)
    {
        values_.clear();
        values_.push_back(v);

        // Notify threads that are waiting for a new value
        cv_.notify_all();
    }

    std::mutex& mutex() { return m_; }
    std::condition_variable& condition_variable() { return cv_; }

private:

    std::vector<double> values_;

    std::mutex m_;

    std::condition_variable cv_;

};

} // end namespace ts

#endif
