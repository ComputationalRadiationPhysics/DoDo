#pragma once


#include <dodo2/graph/TreeIDGraph.hpp>
#include <dodo2/utility/PropertyManager.hpp>
#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>


namespace dodo
{
namespace model
{
namespace hardware
{
namespace extension
{

    using ConsistsOfGraph = graph::TreeIDGraph;

    class VertexSpeed :
        public virtual HardwareAbstractionBase
    {

        std::map<
            ConsistsOfGraph::TreeID,
            size_t
        > id2speed;
        PropertyManager::MapType< decltype( id2speed ) > speedMap;


        VertexSpeed( ) :
            speedMap( id2speed )
        {
            propertyManager.registerProperty(
                "VertexSpeed",
                speedMap
            );
        }

    };

}
}
}
}

