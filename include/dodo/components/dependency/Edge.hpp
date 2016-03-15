#pragma once


#include <boost/fusion/include/define_struct.hpp>

#include "../types.hpp"


BOOST_FUSION_DEFINE_STRUCT(
    (dodo)(components)(dependency),
    Edge,
    (dodo::components::types::PortKey, from)
        (dodo::components::types::PortKey, to)
)
