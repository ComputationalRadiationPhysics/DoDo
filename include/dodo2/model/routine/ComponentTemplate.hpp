#pragma once

#include <vector>
#include <string>

#include <boost/container/flat_set.hpp>

#include <dodo2/model/data/Traits/Directions.hpp>
#include <dodo2/model/routine/Port.hpp>
#include "ComponentBase.hpp"
#include "EffortFunction.hpp"


namespace dodo{
namespace model{
namespace routine{


    template<typename T_SimDom, typename T_Instance>
    class ComponentTemplate
    {
    public:
        using Instance = T_Instance;
        static_assert(
            std::is_base_of<
                ComponentBase,
                Instance
            >::value,
            "The instance type does not inherit from ComponentBase"
        );
        using Directions = typename data::traits::Directions< T_SimDom >::Values;
        std::vector< Port< Directions > > inPorts;
        std::vector< Port< Directions > > outPorts;
        std::string name = " ";
        boost::container::flat_set<std::string> predecessors;
    };


} /* routine */
} /* model */
} /* dodo */


