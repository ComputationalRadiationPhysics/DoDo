#pragma once

#include <vector>
#include <string>
#include <map>
#include <exception>
#include "InPort.hpp"
#include "OutPort.hpp"


namespace dodo
{
namespace components
{
    class Component
    {
    public:
        using PortKey = std::string;
    private:
//        friend class ComponentHandle;
        // Invariant: Input Ports and output ports may not have overlapping namespaces!
        std::map<PortKey, std::shared_ptr<OutPort>> outPorts;
        std::map<PortKey, std::shared_ptr<InPort>> inPorts;

    protected:

        void defineOutPort(Component::PortKey&& k)
        {
            outPorts[k] = std::make_shared<OutPort>();
        }

        void defineInPort(Component::PortKey&& k)
        {
            inPorts[k] = std::make_shared<InPort>();
        }

    public:

        bool hasPort(std::string portName)
        {
            auto hasKey = [&portName](auto& m)
            { return m.find(portName) != m.end(); };

            return hasKey(inPorts) || hasKey(outPorts);
        }

        virtual void run() = 0;
        virtual ~Component(){}

    };



}
}



