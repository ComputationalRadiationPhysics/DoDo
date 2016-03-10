#pragma once


#include <dodo.hpp>

class TestSinkMeta :
    public dodo::components::meta::Interface
{
public:
    TestSinkMeta()
    {
        defineInPort({"in"});
    }

};


