#pragma once

#include <string>
#include <boost/container/flat_set.hpp>

namespace dodo{
namespace model{
namespace routine{
namespace detail{

    class DependencyInfo
    {
    public:
        std::string name;
        boost::container::flat_set< std::string > predecessors;

        template< typename T_ComponentTemplate >
        DependencyInfo(
            T_ComponentTemplate temp
        ) :
            name { temp.name },
            predecessors { temp.predecessors }
        {}
    };
} /* detail */
} /* rountine */
} /* model */
} /* dodo */
