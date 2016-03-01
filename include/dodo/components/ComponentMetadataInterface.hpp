#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include <exception>

#include "types.hpp"
#include "InPort.hpp"
#include "OutPort.hpp"
#include "RingBuffer.hpp"


namespace dodo
{
namespace components
{
    class ComponentMetadataInterface
    {
    public:
        using PortKey = types::PortKey;
    private:
        // Invariant: Input Ports and output ports may not have overlapping namespaces!
        std::set<PortKey> outPorts;
        std::map<PortKey, size_t> inPorts;

    protected:

        void defineOutPort(ComponentMetadataInterface::PortKey&& k)
        {
            outPorts.insert(k);
        }

        void defineInPort(ComponentMetadataInterface::PortKey&& k)
        {
            inPorts[k] = 1;
        }

        void defineInPort(ComponentMetadataInterface::PortKey&& k, const size_t chunksBeforeReady)
        {
            inPorts[k] = chunksBeforeReady;
        }


    public:

        bool hasPort(std::string portName)
        {
            auto hasKey = [&portName](auto& m)
            { return m.find(portName) != m.end(); };

            return hasKey(inPorts) || hasKey(outPorts);
        }

    };



}
}



