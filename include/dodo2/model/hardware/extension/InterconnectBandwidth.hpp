#pragma once

#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/model/hardware/property/Bandwidth.hpp>


namespace dodo
{
namespace model
{
namespace hardware
{
namespace extension
{

    class InterconnectBandwidth :
        public virtual HardwareAbstractionBase
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

        std::size_t
        getTransferTime
        (
            const ConsistsOfGraph::TreeID & from,
            const ConsistsOfGraph::TreeID & to,
            const std::size_t dataBytes
        )
        {

            using Weight = std::size_t;

            using IndexMap = std::map<
                InterconnectGraph::VertexID,
                std::size_t
            >;
            IndexMap indexMap;
            boost::associative_property_map<IndexMap> propmapIndex(indexMap);
            int c=0;
            for( auto v : boost::make_iterator_range( ig.getVertices( ) ) )
            {
                put(propmapIndex, v, c++);
            }

            std::vector< Weight > finalDistances( ig.numVertices( ) );
            auto distanceMap = boost::make_iterator_property_map(
                finalDistances.begin( ),
                propmapIndex
            );

            auto wmap = make_transform_value_property_map(
                [ ]( auto & e )
                {
                    return 1. / e;
                },
                inBWMap
            );

            // TODO: set the weight to Latency + Bandwidth!
            boost::dijkstra_shortest_paths(
                *ig.graph,
                ig.getSBGLID( from ),
                boost::distance_map( distanceMap ).weight_map(wmap)
            );

            return distanceMap[ig.getSBGLID( to )];
        }

    };

} /* extension */
} /* hardware */
} /* model */
} /* dodo */
