#pragma once

#include <dodo.hpp>


class TestSourceMeta :
    public dodo::components::meta::Interface
{
public:
    TestSourceMeta()
    {
        defineOutPort({"out"});
    }

};


