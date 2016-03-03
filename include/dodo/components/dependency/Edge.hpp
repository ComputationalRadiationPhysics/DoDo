#pragma once

#include "../types.hpp"

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
