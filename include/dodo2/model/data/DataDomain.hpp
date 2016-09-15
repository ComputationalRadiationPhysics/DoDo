#pragma once

#include <boost/property_map/property_map.hpp>

#include <dodo2/graph/DataGraph.hpp>
#include <dodo2/utility/OneToNMap.hpp>
#include <dodo2/utility/PropertyManager.hpp>
#include <dodo2/mapping/data2pos/Map.hpp>
#include "SimulationDomain.hpp"


namespace dodo
{
namespace model
{
namespace data
{

    class DataDomain
    {
    public:

        using Graph = graph::DataGraph;
        // TODO: think about moving the map to the Abstraction instead of having
        // it directly inside the DataDomain
        using DataID = Graph::VertexID;
        using PosID = graph::CoordinateGraph::VertexID;
        using Map = utility::OneToNMap<
            DataID,
            PosID
        >;
        Graph g;
        Map map;
        utility::PropertyManager propertyManager;

        std::map<
            DataID,
            float
        > wMap;
        boost::associative_property_map< decltype(wMap) > weightMap;

        DataDomain() :
            g{},
            map{},
            propertyManager{},
            wMap{},
            weightMap{wMap}
        {
            propertyManager.registerProperty(
                "sizeInKB",
                weightMap
            );
        }


        template< typename T >
        auto
        getProperty(
            std::string const & propertyName,
            DataID id
        )
        {
            return propertyManager.get< T >(
                propertyName,
                id
            );
        }

        template< typename T >
        auto
        setProperty(
            std::string const & propertyName,
            DataID id,
            T property
        )
        {
            propertyManager.set(
                propertyName,
                id,
                property
            );
        }


        std::size_t
        size()
        const
        {
            return g.numVertices( );
        }

        auto
        moveDataToPos(
            const DataID d,
            const PosID p
        )
        -> void
        {
            map.moveNToOne( p, d );
        }

        auto
        createDataAtPos(
            const PosID p
        )
        -> DataID
        {
            DataID d = g.addVertex( );
            map.addMapping(
                d,
                p
            );
            return d;
        }

        auto
        getDataAtPos(
            const PosID p
        ) const
        -> DataID
        {
            return map.n2one.at( p );
        }

        auto
        getPosOfData(
            const DataID d
        ) const
        -> std::vector< PosID >
        {
            return map.one2n.at( d );
        }


        auto
        getAllDataPositions( ) const
        {
            return map.one2n;
        }

        auto getDataElements() const
        {
            return g.getVertices();
        }



    };

} /* data */
} /* model */
} /* dodo */
