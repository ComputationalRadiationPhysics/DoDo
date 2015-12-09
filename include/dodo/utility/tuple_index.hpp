#pragma once

namespace dodo
{
namespace utility
{


template<
    typename T,
     typename S,
     int i = std::tuple_size< T >::value - 1
>
struct tuple_index
{
    enum
    {
        value = std::is_same<
            typename std::tuple_element<
                i,
                T
            >::type,
            S
        >::value ? i : tuple_index<
            T,
            S,
            i - 1
        >::value
    };


};

template<
    typename T,
    typename S
>
struct tuple_index<
    T,
    S,
    -1
>
{
    enum
    {
        value = -1
    };

};

} /* utility */
} /* dodo */
