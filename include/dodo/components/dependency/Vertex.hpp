#pragma once


#include "dodo/utility/fusion_pointer_to_member.hpp"
#include "../types.hpp"

//#include <boost/fusion/include/define_struct.hpp>
//
//namespace dodo
//{
//namespace components
//{
//namespace dependency
//{
//
//struct Vertex
//{
//    types::ResourceID id;
//};
//
//}
//}
//}
//
//BOOST_FUSION_DEFINE_STRUCT(
//    (dodo)(components)(dependency),
//    Vertex,
//    (dodo::components::types::ResourceID, id)
//)

namespace dodo
{
namespace components
{
namespace dependency
{

struct Vertex
{
    types::ResourceID id;
};

}
}
}

ADAPT_STRUCT_AND_SAVE_MEMBERPTR(
    dodo::components::dependency::Vertex,
    (dodo::components::types::ResourceID, id)
)
