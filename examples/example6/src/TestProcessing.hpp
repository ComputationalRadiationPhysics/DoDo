#pragma once

#include <dodo.hpp>

namespace dodo
{
namespace components
{

class TestProcessing :
    public ComponentRunInterface
{
public:

    TestProcessing()
    {
        defineInPort("in");
        defineOutPort("out");
    }


    virtual void step() override
    {
        std::cout << "TestProcessing: Taking data from input Port 'in'" << std::endl;
        //Chunk<50> x = this->takeFrom("in");
        // Take data from Input
//        int x = takeFrom("in")

        // reinterpret-cast data to usable format

        // process something

        // re-package in a chunk

        // put into output port (let the output-port handle the sending)
        // Putting into output port should be asynchronous!
        std::cout << "TestProcessing: Putting data in output Port 'out'" << std::endl;
        this->putInto("out", Chunk<50>());
    }


    virtual void enableHook() override
    {

    }


    virtual ~TestProcessing() override
    {
    }
};

}
}



