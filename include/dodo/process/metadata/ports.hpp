#pragma once

namespace dodo
{
namespace process
{
namespace metadata
{

template<typename T_Component>
struct Ports
{
    using In = std::tuple<>;
    enum class InNames {};
    using Out = std::tuple<>;
    enum class OutNames {};
};


} /* metadata */
} /* process */
} /* dodo */

