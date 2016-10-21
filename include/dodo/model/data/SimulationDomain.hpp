#pragma once


#include <dodo/graph/CoordinateGraph.hpp>


namespace dodo
{
namespace model
{
namespace data
{

    class SimulationDomain
    {
    public:
        using Graph = graph::CoordinateGraph;
        Graph g;

        auto
        getCells()
        {
            return g.getVertices();
        }

    };

} /* data */
} /* model */
} /* dodo */
