#pragma once


//#include <boost/hana/for_each.hpp>


namespace dodo
{
namespace utility
{

    // Removed to get rid of all hana dependencies.
    // Kept, because it will come in handy

//    template<
//        typename T_OldTuple,
//        typename T_NewTuple
//    >
//    void
//    convertTuple(
//        const T_OldTuple & oldTuple,
//        T_NewTuple & newTuple
//    )
//    {
//        boost::hana::for_each(
//            newTuple,
//            [ &oldTuple ]( auto & i )
//            {
//                using ElemType = typename std::decay< decltype( i ) >::type;
//                constexpr size_t
//                    srcIndex =
//                    utility::getTupleIndex<
//                        T_OldTuple,
//                        ElemType
//                    >( );
//                i = std::get< srcIndex >( oldTuple );
//            }
//        );
//    }

} /* utility */
} /* dodo */
