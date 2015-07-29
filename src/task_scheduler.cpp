#include <set>
#include <string>
#include <vector>
#include <map>

#include <iostream>

// ----------------------------------------------------------------------------------------------------

struct Data
{
    Data(const std::string& name_) : name(name_) {}
    std::string name;

    bool operator<(const Data& other) const
    {
        return name < other.name;
    }
};

// ----------------------------------------------------------------------------------------------------

struct Process
{
    Process(const std::string& name_) : name(name_) {}

    std::string name;
    std::set<Data> inputs;
    std::set<Data> outputs;
};

// ----------------------------------------------------------------------------------------------------

class System
{

public:

    void addProcess(const Process* p)
    {
        processes_.push_back(p);

        for(auto& d : p->inputs)
            process_inputs_[d].insert(p);

        for(auto& d : p->outputs)
            process_outputs_[d].insert(p);
    }

    void showDependencies()
    {
        for(auto p : processes_)
        {
            std::cout << p->name << std::endl;

            // Check for loops
            bool loop_found = false;

            std::set<const Process*> loop_set;
            std::vector<const Process*> loop_vec;

            loop_set.insert(p);
            loop_vec.push_back(p);

            for(unsigned int i = 0; i < loop_vec.size(); ++i)
            {
                const Process* q = loop_vec[i];

                for(auto& d : q->inputs)
                {
                    auto it = process_outputs_.find(d);
                    if (it == process_outputs_.end())
                        continue;

                    for(const Process* p_producer : it->second)
                    {
                        if (p_producer == p)
                        {
                            loop_found = true;
                            break;
                        }

                        if (loop_set.find(p_producer) == loop_set.end())
                        {
                            loop_vec.push_back(p_producer);
                            loop_set.insert(p_producer);
                        }
                    }
                }
            }

            if (loop_found)
                std::cout << "    LOOP FOUND" << std::endl;
        }
    }

private:

    std::map<Data, std::set<const Process*>> process_inputs_;

    std::map<Data, std::set<const Process*>> process_outputs_;

    std::vector<const Process*> processes_;

};

// ----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    System sys;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Process kinect1("kinect1_driver");
    kinect1.outputs.insert(Data("/kinect1/image"));
    sys.addProcess(&kinect1);

    Process clustering1("kinect1_clustering");
    clustering1.inputs.insert(Data("/kinect1/image"));
    clustering1.inputs.insert(Data("/wm/shapes"));
    clustering1.outputs.insert(Data("/kinect1/clusters"));
    sys.addProcess(&clustering1);

    Process association1("kinect1_association");
    association1.inputs.insert(Data("/kinect1/clusters"));
    association1.inputs.insert(Data("/wm/convex_hulls"));
    association1.outputs.insert(Data("/wm/convex_hulls"));
    sys.addProcess(&association1);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Process kinect2("kinect2_driver");
    kinect2.outputs.insert(Data("/kinect2/image"));
    sys.addProcess(&kinect2);

    Process clustering2("kinect2_clustering");
    clustering2.inputs.insert(Data("/kinect2/image"));
    clustering2.inputs.insert(Data("/wm/shapes"));
    clustering2.outputs.insert(Data("/kinect2/clusters"));
    sys.addProcess(&clustering2);

    Process association2("kinect2_association");
    association2.inputs.insert(Data("/kinect2/clusters"));
    association2.inputs.insert(Data("/wm/convex_hulls"));
    association2.outputs.insert(Data("/wm/convex_hulls"));
    sys.addProcess(&association2);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    sys.showDependencies();

    return 0;
}
