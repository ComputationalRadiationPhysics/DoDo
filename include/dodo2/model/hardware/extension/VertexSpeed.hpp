#pragma once


#include <list>
#include <memory>
#include <map>

#include <boost/property_map/dynamic_property_map.hpp>

#include <dodo2/graph/TreeIDGraph.hpp>
#include <dodo2/utility/PropertyManager.hpp>
#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
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

    class VertexSpeed :
        public virtual HardwareAbstractionBase,
        public ExtensionInterface
    {

        std::map<
            ConsistsOfGraph::TreeID,
            size_t
        > id2speed;
        utility::PropertyManager::MapType< decltype( id2speed ) > speedMap;

    protected:
        virtual
        std::size_t
        countPropertiesInternal( ) override
        {
            return id2speed.size();
        }


    public:

        VertexSpeed( ) :
            id2speed{},
            speedMap( id2speed )
        {
            propertyManager.registerProperty(
                "VertexSpeed",
                speedMap
            );
        }



        void
        setVertexSpeed(
            const ConsistsOfGraph::TreeID & id,
            const size_t s
        )
        {
            propertyManager.set(
                std::string("VertexSpeed"),
                id,
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
                id2speed,
                cog,
                freeList
            );
            dp.property(
                "VertexSpeed",
                associativeMap
            );
        }

    };

}
}
}
}

