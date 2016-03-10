#pragma once


#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace dodo
{
namespace utility
{
template <typename StructName, int N>
struct pointer_to_member_N;
}
}

//Heavily "inspired" from BOOST_FUSION_ADAPT_STRUCT
#define YOUR_NS_SAVE_MEMBERPTR_FILLER_0(X, Y)  ((X, Y)) YOUR_NS_SAVE_MEMBERPTR_FILLER_1
#define YOUR_NS_SAVE_MEMBERPTR_FILLER_1(X, Y)  ((X, Y)) YOUR_NS_SAVE_MEMBERPTR_FILLER_0
#define YOUR_NS_SAVE_MEMBERPTR_FILLER_0_END
#define YOUR_NS_SAVE_MEMBERPTR_FILLER_1_END

#define CREATE_POINTER_TO_MEMBER_TRAIT(R,STRUCT_NAME,INDEX,TYPE_AND_NAME)                          \
template <>                                                                                        \
struct pointer_to_member_N<STRUCT_NAME, INDEX>                                                     \
{                                                                                                  \
static constexpr BOOST_PP_TUPLE_ELEM(2,0,TYPE_AND_NAME)                                            \
STRUCT_NAME::* value = &STRUCT_NAME::BOOST_PP_TUPLE_ELEM(2,1,TYPE_AND_NAME);                       \
};

#define YOUR_NS_SAVE_MEMBERPTR(STRUCT_NAME,MEMBERS)                                                \
namespace dodo {                                                                                   \
namespace utility{                                                                                 \
BOOST_PP_SEQ_FOR_EACH_I(CREATE_POINTER_TO_MEMBER_TRAIT,STRUCT_NAME,BOOST_PP_CAT(YOUR_NS_SAVE_MEMBERPTR_FILLER_0 MEMBERS,_END)) \
}                                                                                                  \
}


#define ADAPT_STRUCT_AND_SAVE_MEMBERPTR(TYPE,MEMBERS)                                              \
    BOOST_FUSION_ADAPT_STRUCT(TYPE,MEMBERS)                                                        \
    YOUR_NS_SAVE_MEMBERPTR(TYPE,MEMBERS)
