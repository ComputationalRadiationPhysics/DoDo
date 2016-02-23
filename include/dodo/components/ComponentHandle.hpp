#pragma once

#include<memory>
#include "Component.hpp"

namespace dodo
{
namespace components
{

class ComponentHandle
{
public:
    std::weak_ptr<Component> component;

    ComponentHandle(const std::weak_ptr<Component> &c) : component(c){}

    auto operator[](std::string portName) const
    {
        assert(component.lock()->hasPort(portName));
        return std::make_pair(component, portName);
    }



};


}
}



