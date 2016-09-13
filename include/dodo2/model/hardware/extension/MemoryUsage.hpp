#pragma once


#include <dodo2/graph/TreeIDGraph.hpp>
#include <dodo2/utility/PropertyManager.hpp>
#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/model/hardware/property/MemoryCapacity.hpp>
#include "ExtensionInterface.hpp"


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
        public virtual HardwareAbstractionBase,
        public ExtensionInterface
    {

        std::map<
            ConsistsOfGraph::TreeID,
//            property::MemoryCapacity
            std::size_t
        > id2memusage;
        utility::PropertyManager::MapType< decltype( id2memusage ) > memUsageMap;

    public:

        MemoryUsage( ) :
            id2memusage{},
            memUsageMap( id2memusage )
        {
            propertyManager.registerProperty(
                std::string("MemoryUsage"),
                memUsageMap
            );
        }

        void
        setCapacity(
            const ConsistsOfGraph::TreeID & id,
            const size_t s
        )
        {
            propertyManager.set(
                std::string("MemoryUsage"),
                id,
//                property::MemoryCapacity( s )
                s
            );
        }


        void
        addPropertyToDPWriter(
            boost::dynamic_properties & dp,
            std::list< std::shared_ptr< void > > & freeList
        ) override
        {
            auto associativeMap = createAssociativeIndexMap(
                id2memusage,
                mhg,
                freeList
            );
            dp.property(
                "MemoryUsage",
                associativeMap
            );
        }

    };

} /* extension */
} /* hardware */
} /* model */
} /* dodo */

