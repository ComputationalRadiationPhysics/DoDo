#pragma once


#include <map>

#include <boost/mpl/string.hpp>
#include <boost/fusion/include/for_each.hpp>

#include "dodo/components/types.hpp"


namespace dodo
{
    namespace components
    {
        namespace meta
        {
            namespace detail
            {
/**
 * Helper function to convert a boost::fusion compile-time map
 * into a runtime-map.
 *
 * @tparam CompileTimeSeq should have types similar to
 *         boost::mpl::int_<T> as keys, so that the keys can
 *         be properly generated
 */
                template<typename CompileTimeSeq, typename T_Seq>
                void initRuntimeSeq(T_Seq &s)
                {
                    boost::fusion::for_each(
                        CompileTimeSeq{},
                        [&](auto i)
                        {
                            s.push_back(i);
                        }
                    );
                }

            }
        }
    }
}

