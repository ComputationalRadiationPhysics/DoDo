#pragma once


#include "IHardwareAbstraction.hpp"

namespace dodo
{
    namespace model{

        namespace hardware
        {
            struct HardwareAbstractionSimple :
                public IHardwareAbstraction
            {

                std::map<
                    graph::TreeIDGraph::TreeID,
                    int
                > id2speed;
                PropertyManager::MapType< decltype( id2speed ) > speedMap;

                HardwareAbstractionSimple() :
                    speedMap(id2speed)
                {
                    propertyManager.registerProperty("VertexSpeed", speedMap);
                }


            };

        }
    }
}

