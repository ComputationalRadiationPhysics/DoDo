#pragma once

#include <vector>

#include <dodo2/graph/CoordinateGraph.hpp>
#include "InData.hpp"


namespace dodo{
namespace model{
namespace routine{

    class EffortFunction
    {
    public:
        virtual
        float
        operator()() = 0;

        virtual ~EffortFunction( );
    };


} /* routine */
} /* model */
} /* dodo */


