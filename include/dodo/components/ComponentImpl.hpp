#pragma once

#include <vector>
#include <string>
#include <map>
#include "InPort.hpp"
#include "OutPort.hpp"


namespace dodo
{
namespace components
{
    class Component
    {
        std::map<std::string, OutPort> outPorts;
        std::map<std::string, InPort> inPorts;
    };

}
}



