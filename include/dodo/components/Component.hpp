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
        // Invariant: Input Ports and output ports may not have overlapping namespaces!
        std::map<PortKey, std::shared_ptr<OutPort>> outPorts;
        std::map<PortKey, std::shared_ptr<InPort>> inPorts;

        void enableBase()
        {
            // spawn a thread that loops over the ports and checks for input
        }


    protected:

        void defineOutPort(Component::PortKey&& k)
        {
            outPorts[k] = std::make_shared<OutPort>();
        }

        void defineInPort(Component::PortKey&& k)
        {
            inPorts[k] = std::make_shared<InPort>();
        }

        template<typename Chunk>
        void putInto(Component::PortKey&& k, const Chunk&& c)
        {
            outPorts.at(k)->put(std::forward<const Chunk>(c));
        }

        auto takeFrom(Component::PortKey&& k)
        {
            return inPorts.at(k)->get();
        }


    public:

        bool hasPort(std::string portName)
        {
            auto hasKey = [&portName](auto& m)
            { return m.find(portName) != m.end(); };

            return hasKey(inPorts) || hasKey(outPorts);
        }

        void enable()
        {
            enableBase();
            enableHook();
        }

        // step() is triggered whenever all inputPorts have a chunk ready
        virtual void step() = 0;
        virtual void enableHook(){};
        virtual ~Component(){}

    };



}
}



