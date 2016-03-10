#pragma once

#include "../types.hpp"
#include <dodo/utility/fusion_pointer_to_member.hpp>


namespace dodo
{
namespace components
{
namespace dependency
{

struct Edge
{
    types::PortKey from;
    types::PortKey to;
};

}
}
}


ADAPT_STRUCT_AND_SAVE_MEMBERPTR(
    dodo::components::dependency::Edge,
    (dodo::components::types::PortKey, from)
    (dodo::components::types::PortKey, to)
)
