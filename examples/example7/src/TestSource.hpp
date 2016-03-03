#pragma once

#include <thread>
#include <chrono>

#include <dodo.hpp>


class TestSource :
    public dodo::components::Interface
{
    TestSourceMeta metadata;

    virtual void step() override {}
    virtual void enableHook() override
    {}

    virtual ~TestSource() override
    {}
};




