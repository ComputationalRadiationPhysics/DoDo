#pragma once

#include <string>

namespace dodo{
namespace model{
namespace routine{

    // alternatively: store the vertex IDs of data domain
    // -> less memory and probably faster, but not as abstract
    //    also more complicated if individual data items move
    template<typename T_Direction>
    class Port
    {
    public:
        T_Direction direction;
        std::string domain;
    };


} /* routine */
} /* model */
} /* dodo */


