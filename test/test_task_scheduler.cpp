#include <task_scheduler/process.h>
#include <task_scheduler/scheduler.h>

#include <unistd.h> // usleep

// ----------------------------------------------------------------------------------------------------

class SourceProcess : public ts::Process
{

public:

    SourceProcess(const std::string& out_name, double sleep_sec) : Process("source " + out_name), out_name_(out_name), sleep_sec_(sleep_sec) {}

    void initialize()
    {
        counter_ = 0;
        RegisterOutput(out_name_, out_);
    }

    void process()
    {
        usleep(sleep_sec_ * 1000000);
//        std::cout << "[" << name() << "] Writing " << counter_ << std::endl;

        ts::Time now = std::chrono::system_clock::now();
        out_.write(counter_, now);
        counter_ += 1;
    }

private:

    int counter_;

    std::string out_name_;

    double sleep_sec_;

    ts::OutputPort out_;

};

// ----------------------------------------------------------------------------------------------------

class SumProcess : public ts::Process
{

public:

    SumProcess(const std::string& in1_name, const std::string& in2_name, const std::string& out_name, double sleep_sec)
        : Process("sum " + in1_name + "+" + in2_name), in1_name_(in1_name), in2_name_(in2_name),out_name_(out_name), sleep_sec_(sleep_sec) {}

    void initialize()
    {
        RegisterInput(in1_name_, in1_, true);
        RegisterInput(in2_name_, in2_, false);
        RegisterOutput(out_name_, out_);
    }

    void process()
    {
        double v1, v2;
        ts::Time t1, t2;
        if (in1_.read(v1, t1))
        {
            if (!in2_.read(v2, t2))
                v2 = 0;

            std::cout << "[" << name() << "] Read " << v1 << " and " << v2 << std::endl;
            usleep(sleep_sec_ * 1000000);
            std::cout << "[" << name() << "] Writing " << v1 + v2 << std::endl;
            out_.write(v1 + v2, t1);
        }
    }

private:

    double sleep_sec_;

    std::string in1_name_, in2_name_, out_name_;

    ts::InputPort in1_, in2_;

    ts::OutputPort out_;

};

// ----------------------------------------------------------------------------------------------------

class SimpleProcess : public ts::Process
{

public:

    SimpleProcess(const std::string& in_name, const std::string& out_name, double sleep_sec)
        : Process("process " + in_name), in_name_(in_name), out_name_(out_name), sleep_sec_(sleep_sec) {}

    void initialize()
    {
        RegisterInput(in_name_, in_, true);
        RegisterOutput(out_name_, out_);
    }

    void process()
    {
        double value;
        ts::Time timestamp;
        if (in_.read(value, timestamp))
        {
            std::cout << "[" << name() << "] Read " << value << std::endl;
            usleep(sleep_sec_ * 1000000);
            std::cout << "[" << name() << "] Wrote " << value << std::endl;
            out_.write(value, timestamp);
        }
    }

private:

    std::string in_name_, out_name_;

    double sleep_sec_;

    ts::InputPort in_;

    ts::OutputPort out_;

};

// ----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    std::cout << "Starting main" << std::endl;

    ts::Scheduler scheduler;

    scheduler.AddProcess(new SourceProcess("kinect1", 0.11));
    scheduler.AddProcess(new SourceProcess("kinect2", 0.1));
    scheduler.AddProcess(new SumProcess("kinect1", "wm", "wm", 0.09));
    scheduler.AddProcess(new SumProcess("kinect2", "wm", "wm", 0.09));
//    scheduler.AddProcess(new SimpleProcess("kinect2", "wm", 0.05));



//    std::cout << "Starting system..." << std::endl;

    scheduler.Run();

    usleep(20000000);

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
