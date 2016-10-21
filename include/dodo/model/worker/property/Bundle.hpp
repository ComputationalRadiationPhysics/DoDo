#pragma once

#include "NodeType.hpp"

namespace dodo
{
namespace model
{
namespace worker
{
namespace property
{

    struct Bundle
    {
        std::size_t id;
        NodeType type;
        unsigned padding;
    };

} /* property */
} /* worker */
} /* model */
} /* dodo */
