#pragma once


#include <dodo2/graph/CoordinateGraph.hpp>


namespace dodo
{
namespace model
{
namespace data
{

    template<typename T_Derived>
    class SimulationDomain
    {
    protected:
        std::map<
            graph::CoordinateGraph::EdgeID,
            T_Derived::Directions
        > edge2Direction;
        boost::associative_property_map< decltype(edge2Direction) > directionMap;

        graph::CoordinateGraph g;





    };

} /* data */
} /* model */
} /* dodo */
