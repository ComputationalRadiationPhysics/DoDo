#pragma once

namespace dodo
{
    namespace process
    {

// template<typename T_PortMeta>
        struct ComponentMeta
        {
            // using PortMeta = T_PortMeta;
            using nOutPorts = std::integral_constant<unsigned, 0>;
            enum InNames
            {
            };
            using nInPorts = std::integral_constant<unsigned, 0>;
            enum OutNames
            {
            };
            // std::tuple<PortMeta> inPortMeta;
            // std::tuple<PortMeta> outPortMeta;
        };

    } /* process */
} /* dodo */
