#pragma once

#include <dodo.hpp>

namespace dodo
{
namespace components
{

class TestSink : public Component
{
public:

    TestSink()
    {
        defineInPort("in");
    }

    virtual void step() override
    {
        std::cout << "TestSink: Taking data from input Port 'in'" << std::endl;
        // Take Data from IN port

        // reinterpret-cast data to correct format

        // print Data
    }

    virtual ~TestSink() override {}
};


}
}


