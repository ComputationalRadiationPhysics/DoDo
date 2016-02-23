#pragma once

#include "Component.hpp"

namespace dodo
{
namespace components
{

class TestSource : public Component
{
public:

    TestSource()
    {
        defineOutPort("A");
    }

    virtual void run() override
    {
        for(int i=0; i<20; ++i)
        {
//            outPorts["A"].send(std::to_string(i));
        }
    }

    virtual ~TestSource() override {}
};


}
}


