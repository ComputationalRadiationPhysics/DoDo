#pragma once

#include <vector>
#include <string>

#include <dodo2/model/data/Traits/Directions.hpp>
#include <dodo2/model/routine/Port.hpp>


namespace dodo{
namespace model{
namespace routine{


    template<typename T_SimDom>
    class ComponentTemplate
    {
    public:
        using Directions = typename data::traits::Directions< T_SimDom >;
        std::vector< Port< Directions > > inPorts;
        std::vector< Port< Directions > > outPorts;
        std::string name;
    };


} /* routine */
} /* model */
} /* dodo */


