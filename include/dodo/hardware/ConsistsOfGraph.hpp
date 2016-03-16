#pragma once

#include <boost/fusion/include/define_struct.hpp>

#include "dodo/graph/SimpleBGL.hpp"
#include "dodo/utility/tree_id.hpp"

namespace dodo
{
    namespace hardware
    {

        class ConsistsOfGraph :
            public graph::SimpleBGL<
                utility::TreeID
            >
        {};


    }
}
