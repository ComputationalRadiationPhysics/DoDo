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
        // TODO: think about moving the map to the Abstraction instead of having
        // it directly inside the DataDomain
        using Map = utility::OneToNMap<graph::CoordinateGraph, Graph >;
        using DataID = Graph::VertexID;
        Graph g;
        Map map;
        bool readOnly;
        SimulationDomain* simDom;
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
