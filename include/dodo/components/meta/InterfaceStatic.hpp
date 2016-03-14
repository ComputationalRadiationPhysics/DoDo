#pragma once

#include <vector>
#include <map>
#include <set>
#include <exception>

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/string.hpp>

#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/hana.hpp>


#include "dodo/components/types.hpp"
#include "dodo/components/meta/detail/InitRuntimeMap.hpp"
#include "dodo/components/meta/traits.hpp"
#include "dodo/components/meta/Interface.hpp"


namespace dodo
{
namespace components
{
namespace meta
{


/**
 * Main reason for this interface at the time:
 * Enforce existance of certain traits.
 *
 * Inheriting from "Interface" is not 100% necessary,
 * the same could be done using multiple inheritance for the derived class
 */
template<typename T_Derived>
class InterfaceStatic : public Interface
{
public:
    using InPorts = typename traits::InPortMap<T_Derived>::value;
    using OutPorts = typename traits::OutPortMap<T_Derived>::value;

    InterfaceStatic()
    {
        testNoIdenticalPorts();
        detail::initRuntimeMap<InPorts>(inPortMap);
        detail::initRuntimeMap<OutPorts>(outPortMap);
    }

private:
    constexpr void testNoIdenticalPorts(){
        boost::fusion::for_each(
            InPorts(),
            [&](auto i){
                using Elem = decltype(i);
                using Key = typename Elem::first;
                static_assert(
                    ! boost::mpl::has_key<OutPorts, Key>::type::value,
                    "Input and Output ports may not have the same name!"
                );
            }
        );
    }

};



}
}
}

