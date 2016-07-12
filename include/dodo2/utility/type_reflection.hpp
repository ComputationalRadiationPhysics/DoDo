#pragma once


#include <boost/fusion/include/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/bind.hpp>
#include "tuple_index.hpp"


namespace dodo
{
namespace utility
{

    template< typename T_Types>
    class type_reflection
    {

        template<
    typename T,
    //                typename Base,
    //                typename = std::enable_if <std::is_base_of<
    //                    T,
    //                    Base
    //                >::value>,
    typename ... T_Args

    >
        static
        T *
        create( T_Args... args )
        {
            auto x = new T( args... );
            x->typeindex =
                utility::tuple_index<
                    T_Types,
                    T
                >::value;
            return x;
        }


        template<
    template< typename > class T_Struct,
    typename Base,
    typename... Args
    >
        static
        void
        applyStruct(
            Base * derived,
            Args & ... args
        )
        {
            using Range = boost::mpl::range_c<
                unsigned,
                0,
                boost::fusion::result_of::size< T_Types >::value
            >;
            boost::fusion::for_each(
                Range{ },
                [ & ]( auto j )
                {
                    if( derived->typeindex == j )
                    {
                        using T = typename boost::mpl::at<
                            T_Types,
                            boost::mpl::int_< j >
                        >::type;
                        T_Struct< T >( )(
                            derived,
                            args...
                        );
                    }
                }
            );
        }
    };


} /* utility */
} /* dodo */

