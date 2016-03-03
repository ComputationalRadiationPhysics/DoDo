#pragma once

#include <dodo.hpp>


class TestProcessing :
    public dodo::components::Interface
{
    TestSourceMeta metadata;
public:

    virtual void step() override
    {
    }

    virtual void enableHook() override
    {
    }


    virtual ~TestProcessing() override
    {
    }
};



