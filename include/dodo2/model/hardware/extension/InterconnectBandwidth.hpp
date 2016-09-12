#pragma once

#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/model/hardware/property/Bandwidth.hpp>
#include "ExtensionInterface.hpp"


namespace dodo
{
namespace model
{
namespace hardware
{
namespace extension
{

    class InterconnectBandwidth :
        public virtual HardwareAbstractionBase,
        public ExtensionInterface
    {
        std::map<
            InterconnectGraph::EdgeID,
            std::size_t
        > id2inBW;
        utility::PropertyManager::MapType< decltype( id2inBW ) > inBWMap;

        std::map<
            InterconnectGraph::EdgeID,
            std::size_t
        > id2inLat;
        utility::PropertyManager::MapType< decltype( id2inLat ) > inLatMap;


    protected:
        InterconnectBandwidth( ) :
            inBWMap( id2inBW ),
            inLatMap( id2inLat )
        {
            propertyManager.registerProperty(
                "InterconnectBandwidth",
                inBWMap
            );
            propertyManager.registerProperty(
                "InterconnectLatency",
                inLatMap
            );
        }

    public:

        float
        getTransferTime
        (
            const ConsistsOfGraph::TreeID & from,
            const ConsistsOfGraph::TreeID & to,
            const std::size_t dataBytes
        )
        {


            std::vector< float >distanceMem( ig.numVertices( ) );
            auto distanceMap = boost::make_iterator_property_map(
                distanceMem.begin( ),
                boost::get(boost::vertex_index, *ig.graph)
            );

            std::map< InterconnectGraph::EdgeID, float > durationMem;
            for(auto eIt : id2inBW)
            {
                InterconnectGraph::EdgeID e = eIt.first;
                float time = id2inLat[e] + static_cast<float>(dataBytes) / eIt.second;
                durationMem.insert(std::make_pair(e, time));
            }
            boost::associative_property_map<decltype(durationMem)> transferDuration(durationMem);


            boost::dijkstra_shortest_paths(
                *ig.graph,
                ig.getSBGLID( from ),
                boost::distance_map( distanceMap ).weight_map(transferDuration)
            );

            return distanceMap[ig.getSBGLID( to )];
        }

        virtual
        void
        addPropertyToDPWriter(
            boost::dynamic_properties & dp,
            std::list< std::shared_ptr< void > > &
        ) override
        {
            dp.property(
                "InterconnectBandwidth",
                inBWMap
            );

            dp.property(
                "InterconnectLatency",
                inLatMap
            );
        }

        virtual ~InterconnectBandwidth( ) = default;

    };

} /* extension */
} /* hardware */
} /* model */
} /* dodo */
