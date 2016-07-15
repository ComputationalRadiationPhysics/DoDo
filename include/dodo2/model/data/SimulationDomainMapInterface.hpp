#pragma once


#include <dodo2/graph/CoordinateGraph.hpp>


namespace dodo
{
namespace model
{
namespace data
{

    template<typename T_Derived>
    class SimulationDomainMapInterface
    {
    protected:
        std::map<
            graph::CoordinateGraph::EdgeID,
            T_Derived::Directions
        > edge2Direction;
        boost::associative_property_map< decltype(edge2Direction) > directionMap;
    };

} /* data */
} /* model */
} /* dodo */
