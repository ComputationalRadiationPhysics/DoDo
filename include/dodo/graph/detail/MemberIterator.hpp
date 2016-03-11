#pragma once


#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/phoenix/fusion/at.hpp>
#include <boost/phoenix/function.hpp>
#include <boost/phoenix/core/argument.hpp>

#include "PrettyPrinter.hpp"

namespace dodo
{
namespace graph
{
namespace detail
{

/**
 * Iterate put the members of a struct into a boost::dynamic_properties object
 *
 * Takes a graph and tansforms all members of a specified tag into a printable
 * property map. Each member type must have an overload for the
 * dodo::graph::detail::prettyPrinter<T> function
 *
 * @tparam T_Tag the tag where to collect the member from. Can be vertex_bundle_t or edge_bundle_t
 * @param dp the dynamic_properties which will be filled
 * @param g the BGL graph where the properties come from
 */
template <typename T_Tag, typename T_Graph>
void
member_iterator(
    boost::dynamic_properties& dp,
    T_Graph& g
){
    using namespace boost;

    using PropMap = typename property_map<T_Graph, T_Tag>::type;
    using MemberSeq = typename property_traits<PropMap>::value_type;
    using Indices = mpl::range_c<unsigned, 0, fusion::result_of::size<MemberSeq>::value>;

    fusion::for_each(
        Indices{},
        [&](auto i)
        {
            const auto name = fusion::extension::struct_member_name<MemberSeq, i>::call();
            phoenix::function<PrettyPrinter> prettyPrinter;

            PropMap propMap = get( T_Tag{}, g );
            dp.property(
                name,
                make_transform_value_property_map(
                    prettyPrinter(
                        phoenix::at_c<i>( phoenix::arg_names::arg1 )
                    ),
                    propMap
                )
            );
        }
    );
}

}
}
}

