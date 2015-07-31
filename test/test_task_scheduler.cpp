#include <task_scheduler/process.h>
#include <task_scheduler/scheduler.h>

#include <unistd.h> // usleep

// ----------------------------------------------------------------------------------------------------

class SourceProcess : public ts::Process
{

public:

    SourceProcess(const std::string& out_name) : Process("source"), out_name_(out_name) {}

    void initialize()
    {
        counter_ = 0;
        RegisterOutput(out_name_, out_);
    }

    void process()
    {
        std::cout << "[" << name() << "] Writing " << counter_ << std::endl;
        out_.write(counter_++);
        usleep(1000000);
    }

private:

    int counter_;

    std::string out_name_;

    ts::OutputPort out_;

};


// ----------------------------------------------------------------------------------------------------

class SimpleProcess : public ts::Process
{

public:

    SimpleProcess(const std::string& in_name, const std::string& out_name)
        : Process("process_" + in_name), in_name_(in_name), out_name_(out_name) {}

    void initialize()
    {
        RegisterInput(in_name_, in_, true);
        RegisterOutput(out_name_, out_);
    }

    void process()
    {
        double value;
        if (in_.read(value))
        {
            std::cout << "[" << name() << "] Read " << value << std::endl;
            out_.write(value);
        }
    }

private:

    std::string in_name_, out_name_;

    ts::InputPort in_;

    ts::OutputPort out_;

};

// ----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    std::cout << "Starting main" << std::endl;

    ts::Scheduler scheduler;
    scheduler.AddProcess(new SourceProcess("a"));
    scheduler.AddProcess(new SimpleProcess("a", "b"));
    scheduler.AddProcess(new SimpleProcess("b", "c"));

//    std::cout << "Starting system..." << std::endl;

    scheduler.Run();

    usleep(10000000);

//    scheduler.blackboard().PrintContent();

//    SourceProcess bla("a");
//    bla.run();


//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Process kinect1("kinect1_driver");
//    kinect1.outputs.insert(Data("/kinect1/image"));
//    sys.addProcess(&kinect1);

//    Process clustering1("kinect1_clustering");
//    clustering1.inputs.insert(Data("/kinect1/image"));
//    clustering1.inputs.insert(Data("/wm/shapes"));
//    clustering1.outputs.insert(Data("/kinect1/clusters"));
//    sys.addProcess(&clustering1);

//    Process association1("kinect1_association");
//    association1.inputs.insert(Data("/kinect1/clusters"));
//    association1.inputs.insert(Data("/wm/convex_hulls"));
//    association1.outputs.insert(Data("/wm/convex_hulls"));
//    sys.addProcess(&association1);

//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    Process kinect2("kinect2_driver");
//    kinect2.outputs.insert(Data("/kinect2/image"));
//    sys.addProcess(&kinect2);

//    Process clustering2("kinect2_clustering");
//    clustering2.inputs.insert(Data("/kinect2/image"));
//    clustering2.inputs.insert(Data("/wm/shapes"));
//    clustering2.outputs.insert(Data("/kinect2/clusters"));
//    sys.addProcess(&clustering2);

//    Process association2("kinect2_association");
//    association2.inputs.insert(Data("/kinect2/clusters"));
//    association2.inputs.insert(Data("/wm/convex_hulls"));
//    association2.outputs.insert(Data("/wm/convex_hulls"));
//    sys.addProcess(&association2);

//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//    sys.showDependencies();

    return 0;
}
