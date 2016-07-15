#pragma once


#include <dodo2/graph/DataGraph.hpp>
#include <dodo2/utility/OneToNMap.hpp>
#include <dodo2/utility/PropertyManager.hpp>
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
        using Map = utility::OneToNMap<graph::CoordinateGraph, Graph >;
        Graph g;
        Map map;
        bool readOnly;
        SimulationDomain* simDom;
        utility::PropertyManager propertyManager;

        std::map<
            Graph::VertexID,
            float
        > wMap;
        boost::associative_property_map< decltype(wMap) > weightMap;

        DataDomain()
        {
            propertyManager.registerProperty("weight", weightMap);
        }


        std::size_t
        size() const
        {
            return g.numVertices();
        }
    };

} /* data */
} /* model */
} /* dodo */
