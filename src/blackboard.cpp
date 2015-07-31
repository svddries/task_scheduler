#include "task_scheduler/blackboard.h"

//#include <iostream>

namespace ts
{

// ----------------------------------------------------------------------------------------------------

Blackboard::Blackboard()
{
}

// ----------------------------------------------------------------------------------------------------

Blackboard::~Blackboard()
{
    for(auto c : channels_)
        delete c;
}

// ----------------------------------------------------------------------------------------------------

void Blackboard::PrintContent()
{
//    for(auto& p : name_to_channel_)
//    {
//        std::cout << p.first << ": ";

//        DataChannel* c = channels_[p.second.idx];

//        double v;
//        if (c->read(v))
//            std::cout << v << std::endl;
//        else
//            std::cout << "-" << std::endl;
//    }
}

} // end namespace ts

