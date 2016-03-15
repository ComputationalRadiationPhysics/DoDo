#pragma once


#include <vector>
#include <string>
#include <map>
#include <memory>
#include <exception>
#include "dodo/components/meta/Interface.hpp"
#include "dodo/components/meta/InterfaceStatic.hpp"
#include "dodo/components/InPort.hpp"
#include "dodo/components/OutPort.hpp"
#include "dodo/utility/RingBuffer.hpp"


namespace dodo
{
    namespace components
    {
        namespace run
        {

            class Interface
            {
                using PortKey = meta::Interface::PortKey;
            private:
                std::shared_ptr<meta::Interface> metadata;
                std::map<PortKey, std::shared_ptr<utility::RingBuffer>> outBuffers;
                std::map<PortKey, std::shared_ptr<utility::RingBuffer>> inBuffers;


                void enableBase()
                {
                    // spawn a thread that loops over the ports and checks for input
                    // if all ports are ready, execute the "step()" method
                }


            protected:


                template<typename Chunk>
                void putInto(Interface::PortKey &&, const Chunk &&)
                {
//            outBuffers.at(k)->put(std::forward<const Chunk>(c));
                }


                auto takeFrom(Interface::PortKey &&)
                {
//            return inPorts.at(k)->get();
                }


            public:
                // Force initialization of metadata in derived class
                Interface() = delete;


                Interface(std::shared_ptr<meta::Interface> meta) :
                    metadata(meta)
                {
                }


                void enable()
                {
                    enableBase();
                    enableHook();
                }


                // step() is triggered whenever all inputPorts have a chunk ready
                virtual void step() = 0;


                virtual void enableHook()
                {
                };


                virtual ~Interface()
                {
                }

            };


        }
    }
}



