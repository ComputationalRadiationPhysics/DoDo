#pragma once


#include <dodo2/graph/TreeIDGraph.hpp>
#include <dodo2/model/hardware/PropertyManager.hpp>
#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/model/hardware/property/MemoryCapacity.hpp>


namespace dodo
{
namespace model
{
namespace hardware
{
namespace extension
{

    using ConsistsOfGraph = graph::TreeIDGraph;

    class MemoryUsage :
        public virtual HardwareAbstractionBase
    {

        std::map<
            ConsistsOfGraph::TreeID,
            property::MemoryCapacity
        > id2memusage;
        PropertyManager::MapType< decltype( id2memusage ) > memUsageMap;


        MemoryUsage( ) :
            memUsageMap( id2memusage )
        {
            propertyManager.registerProperty(
                "MemoryUsage",
                memUsageMap
            );
        }

    };

} /* extension */
} /* hardware */
} /* model */
} /* dodo */

