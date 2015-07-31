#ifndef TASK_SCHEDULER_SCHEDULER_H_
#define TASK_SCHEDULER_SCHEDULER_H_

#include "task_scheduler/types.h"
#include "task_scheduler/blackboard.h"

#include <vector>

namespace ts
{

class Scheduler
{

public:

    Scheduler();

    ~Scheduler();

    void AddProcess(Process* p);

    void Run();

    Blackboard& blackboard() { return blackboard_; }

private:

    Blackboard blackboard_;

    std::vector<Process*> processes_;

};

} // end namespace ts

#endif


//class System
//{

//public:

//    void addProcess(const Process* p)
//    {
//        processes_.push_back(p);

//        for(auto& d : p->inputs)
//            process_inputs_[d].insert(p);

//        for(auto& d : p->outputs)
//            process_outputs_[d].insert(p);
//    }

//    void showDependencies()
//    {
//        // 1) Start with some process p
//        // 2) Do breadth-first-search on all processes producing input (recursively)
//        // 3) If it returns to process p, a loop is detected. Put all entities in the loop in a group.
//        // ...



//        for(auto p : processes_)
//        {
//            std::cout << p->name << std::endl;

//            // Check for loops
//            bool loop_found = false;

//            std::set<const Process*> loop_set;
//            std::vector<const Process*> loop_vec;

//            loop_set.insert(p);
//            loop_vec.push_back(p);

//            for(unsigned int i = 0; i < loop_vec.size(); ++i)
//            {
//                const Process* q = loop_vec[i];

//                for(auto& d : q->inputs)
//                {
//                    auto it = process_outputs_.find(d);
//                    if (it == process_outputs_.end())
//                        continue;

//                    for(const Process* p_producer : it->second)
//                    {
//                        if (p_producer == p)
//                        {
//                            loop_found = true;
//                            break;
//                        }

//                        if (loop_set.find(p_producer) == loop_set.end())
//                        {
//                            loop_vec.push_back(p_producer);
//                            loop_set.insert(p_producer);
//                        }
//                    }
//                }
//            }

//            if (loop_found)
//                std::cout << "    LOOP FOUND" << std::endl;
//        }
//    }

//private:

//    std::map<Data, std::set<const Process*>> process_inputs_;

//    std::map<Data, std::set<const Process*>> process_outputs_;

//    std::vector<const Process*> processes_;

//};
