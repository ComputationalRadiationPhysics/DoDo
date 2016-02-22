#pragma once

#include "OutPort.hpp"


namespace dodo
{
namespace components
{

    class InPort : public Port
    {
        // Adds a new connection
        void dependsOn(OutPort*);
    };


    void InPort::dependsOn(OutPort*)
    {

    }
}
}
