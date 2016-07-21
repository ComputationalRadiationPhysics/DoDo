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
            PosID,
            DataID
        >;
        Graph g;
        Map map;
        bool readOnly;
//        SimulationDomain* simDom;
        utility::PropertyManager propertyManager;
        std::string name;

        std::map<
            DataID,
            float
        > wMap;
        boost::associative_property_map< decltype(wMap) > weightMap;

        DataDomain(const std::string& name)
            : name(name)
        {
            propertyManager.registerProperty(
                "weight",
                weightMap
            );
        }



        std::size_t
        size() const
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
            map.moveNToOne( d, p );
        }

        auto
        createDataAtPos(
            const PosID p
        )
        -> DataID
        {
            auto d = g.addVertex( );
            map.addMapping(
                p,
                d
            );
            return d;
        }

        auto
        getDataAtPos(
            const PosID p
        ) const
        -> std::vector< DataID >
        {
            return map.one2n.at( p );
        }

        auto
        getPosOfData(
            const DataID d
        ) const
        -> PosID
        {
            return map.n2one.at( d );
        }


        auto
        getDataAtAllPos( ) const
        {
            return map.one2n;
        }



    };

} /* data */
} /* model */
} /* dodo */
