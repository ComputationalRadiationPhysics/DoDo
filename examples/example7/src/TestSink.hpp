#pragma once

#include <dodo.hpp>


class TestSink :
    public dodo::components::ComponentRunInterface
{

    TestSourceMeta metadata;
public:

    virtual void step() override
    {
    }

    virtual ~TestSink() override {}
};




