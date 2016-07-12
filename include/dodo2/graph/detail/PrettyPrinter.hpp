#pragma once


#include <string>
#include <sstream>


namespace dodo
{
namespace graph
{
namespace detail
{

    struct PrettyPrinter
    {
        using result_type = std::string;


        template<typename T>
        result_type
        operator()(T &&v) const
        {
            return toString(std::forward<T>(v));
        }
    };

} /* detail */
} /* graph */
} /* dodo */

