#pragma once

#include "Component.hpp"

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

    virtual void run() override
    {
        for(int i=0; i<20; ++i)
        {
            //outPorts["A"].send(std::to_string(i));
        }
    }

    virtual ~TestSink() override {}
};


}
}


