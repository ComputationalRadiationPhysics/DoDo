#pragma once

#include <dodo.hpp>


class TestSink :
    public dodo::components::Interface
{

    TestSourceMeta metadata;
public:

    virtual void step() override
    {
    }

    virtual ~TestSink() override {}
};




