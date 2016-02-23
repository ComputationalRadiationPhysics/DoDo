#pragma once

#include "Component.hpp"

namespace dodo
{
namespace components
{

class TestProcessing : public Component
{
public:

    TestProcessing()
    {
        defineInPort("in");
        defineOutPort("out");
    }

    virtual void run() override
    {
        for(int i=0; i<20; ++i)
        {
            //outPorts["A"].send(std::to_string(i));
        }
    }

    virtual ~TestProcessing() override {}
};


}
}


