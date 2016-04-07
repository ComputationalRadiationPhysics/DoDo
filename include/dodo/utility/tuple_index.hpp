#pragma once

namespace dodo
{
    namespace utility
    {


        template<
            typename Tuple,
            typename Elem,
            int i = std::tuple_size<Tuple>::value - 1
        >
        struct tuple_index
        {
            enum
            {
                value = std::is_same<
                    typename std::tuple_element<
                        i,
                        Tuple
                    >::type,
                    Elem
                >::value ? i : tuple_index<
                    Tuple,
                    Elem,
                    i - 1
                >::value
            };


        };

        template<
            typename Tuple,
            typename Elem
        >
        struct tuple_index<
            Tuple,
            Elem,
            -1
        >
        {
            enum
            {
                value = -1
            };

        };


        template<
            typename Tuple,
            typename Elem
        >
        constexpr int getTupleIndex()
        {
            constexpr int x{tuple_index<Tuple, Elem>::value};
            static_assert(x >= 0, "Type Mismatch: The tuple does not contain the type");
            return x;
        }


    } /* utility */
} /* dodo */
