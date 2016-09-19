#pragma once

#include <vector>

#include <dodo2/graph/CoordinateGraph.hpp>
#include "InData.hpp"


namespace dodo{
namespace model{
namespace routine{

    class ComponentBase
    {
    protected:
        using PosID = data::SimulationDomain::Graph::VertexID;
        PosID id;
        std::vector< InData > inData;
        std::vector< InData > outData;

    public:
        virtual
        float
        effort() = 0;
    };


} /* routine */
} /* model */
} /* dodo */


