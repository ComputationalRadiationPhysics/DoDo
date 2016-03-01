#pragma once

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <exception>
#include "ComponentMetadataInterface.hpp"
#include "InPort.hpp"
#include "OutPort.hpp"
#include "RingBuffer.hpp"


namespace dodo
{
namespace components
{

    class ComponentRunInterface
    {
        using PortKey = ComponentMetadataInterface::PortKey;
    private:
        std::shared_ptr<ComponentMetadataInterface> metadata;
        std::map<PortKey, std::shared_ptr<RingBuffer>> outBuffers;
        std::map<PortKey, std::shared_ptr<RingBuffer>> inBuffers;

        void enableBase()
        {
            // spawn a thread that loops over the ports and checks for input
            // if all ports are ready, execute the "step()" method
        }


    protected:


        template<typename Chunk>
        void putInto(ComponentRunInterface::PortKey&& k, const Chunk&& c)
        {
//            outBuffers.at(k)->put(std::forward<const Chunk>(c));
        }

        auto takeFrom(ComponentRunInterface::PortKey&& k)
        {
//            return inPorts.at(k)->get();
        }


    public:
        // Force initialization of metadata in derived class
        ComponentRunInterface() = delete;
        ComponentRunInterface(std::shared_ptr<ComponentMetadataInterface> meta) :
            metadata(meta)
        {}

        void enable()
        {
            enableBase();
            enableHook();
        }

        // step() is triggered whenever all inputPorts have a chunk ready
        virtual void step() = 0;
        virtual void enableHook(){};
        virtual ~ComponentRunInterface(){}

    };



}
}



