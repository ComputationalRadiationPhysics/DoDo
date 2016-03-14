#pragma once

#include <vector>
#include <map>
#include <set>
#include <exception>

#include "dodo/components/types.hpp"


namespace dodo
{
namespace components
{
namespace meta
{

class Interface
{
public:
    using PortKey = types::PortKey;

    std::map<PortKey::value_type, int> inPortMap;
    std::map<PortKey::value_type, int> outPortMap;

    bool hasPort(PortKey::value_type portName) {
        auto hasKey = [&portName](auto &m)
        { return m.find({portName}) != m.end(); };

        return hasKey(inPortMap) || hasKey(outPortMap);
    }
};

}
}
}



