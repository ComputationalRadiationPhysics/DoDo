#pragma once


#include "../graph/SimpleBGL.hpp"

namespace dodo
{
namespace components
{


class DependencyBGL :
    public graph::SimpleBGL<
        boost::no_property,
        Connection,
        boost::vecS,
        boost::listS
    >{};


}
}

