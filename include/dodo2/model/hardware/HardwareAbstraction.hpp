#pragma once


#include "IHardwareAbstraction.hpp"

namespace dodo
{
    namespace model{

        namespace hardware
        {
            struct HardwareAbstraction:
                public IHardwareAbstraction
            {

                std::map<
                    graph::TreeIDGraph::TreeID,
                    int
                > id2speed;
                boost::associative_property_map< decltype( id2speed ) > speedMap;

                HardwareAbstraction() :
                    speedMap(id2speed)
                {
                    properties.property("VertexSpeed", speedMap);
                }


            };

        }
    }
}

