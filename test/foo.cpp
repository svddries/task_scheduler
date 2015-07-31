#include <iostream>
#include <boost/thread.hpp>

void foo()
{

}

void bar(int x)
{

}

int main(int argc, char **argv)
{
    boost::thread first(foo);
    boost::thread second(bar, 0);

    std::cout << "Blaaaa" << std::endl;

    first.join();
    second.join();

    std::cout << "Done" << std::endl;

    return 0;
}
