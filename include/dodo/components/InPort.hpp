#pragma once

#include "OutPort.hpp"


namespace dodo
{
namespace components
{

    class InPort : public Port
    {
        // Adds a new connection
        void dependsOn(OutPort* dependency);
    };


    void InPort::dependsOn(OutPort *dependency)
    {

    }
}
}
