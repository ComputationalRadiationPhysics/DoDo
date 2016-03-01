#pragma once

#include <thread>
#include <chrono>

#include <dodo.hpp>

namespace dodo
{
namespace components
{

class TestSource : public ComponentRunInterface
{
    std::thread sourceThread;
public:

    TestSource()
    {
        defineOutPort("A");
    }

    //TODO: Find a way to generate continous output
    // Maybe start a thread within the constructor that calls the run function?

    virtual void step() override {}
    virtual void enableHook() override
    {
        sourceThread = std::thread(
            [this]()
            {
                for(int i=0 ; i<10 ; ++i)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(350));
                    std::cout << "TestSource: Putting data into output Port A" << std::endl;
                    this->putInto("A", Chunk<50>());
                    // TODO
                }

            }
        );
    }

    virtual ~TestSource() override {
        sourceThread.join();
    }
};


}
}


