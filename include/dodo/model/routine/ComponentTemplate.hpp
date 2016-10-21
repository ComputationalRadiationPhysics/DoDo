#pragma once

#include <vector>
#include <string>

#include <boost/container/flat_set.hpp>

#include <dodo/model/data/Traits/Directions.hpp>
#include <dodo/model/routine/Port.hpp>
#include "ComponentBase.hpp"
#include "EffortFunction.hpp"


namespace dodo{
namespace model{
namespace routine{


    //TODO: define predecessor class that holds a direction.
    //      With that class, it is possible to actually depend
    //      on component instances that are located on a different
    //      position in the simulation domain.
    //      Of course, this has to be handled correctly in the
    //      dependency generation step, but should be rather trivial.
    using Predecessor = std::string;

    template<
        typename T_Instance
    >
    class ComponentTemplate
    {
    public:
        using Instance = T_Instance;
        static_assert(
            std::is_base_of<
                ComponentBase < typename Instance::DataModel >,
                Instance
            >::value,
            "The instance type does not inherit from ComponentBase"
        );
        using Directions = typename Instance::DataModel::Directions;
        std::vector< Port< Directions > > inPorts;
        std::vector< Port< Directions > > outPorts;
        std::string name = " ";
        boost::container::flat_set< Predecessor > predecessors;
    };


} /* routine */
} /* model */
} /* dodo */


