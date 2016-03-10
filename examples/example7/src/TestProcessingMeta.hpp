#pragma once

#include <dodo.hpp>

class TestProcessingMeta :
    public dodo::components::meta::Interface
{
public:
    TestProcessingMeta()
    {
        defineInPort({"in"});
        defineOutPort({"out"});
    }

};

