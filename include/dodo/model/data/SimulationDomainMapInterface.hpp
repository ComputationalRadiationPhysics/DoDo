#pragma once


#include <dodo/graph/CoordinateGraph.hpp>
#include "Traits/Directions.hpp"


namespace dodo
{
namespace model
{
namespace data
{

    template<typename T_Derived>
    class SimulationDomainMapInterface
    {
    public:
        using PosID = typename graph::CoordinateGraph::VertexID;
        using Directions = typename traits::Directions<T_Derived>::Values;

        virtual
        auto
        getNeighbor(
            PosID const,
            Directions const
        ) const
        -> PosID = 0;

        SimulationDomainMapInterface() = default;
        SimulationDomainMapInterface(const SimulationDomainMapInterface& ) = default;
        virtual ~SimulationDomainMapInterface( ){}
    };

} /* data */
} /* model */
} /* dodo */
