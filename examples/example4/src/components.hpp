#pragma once

#include <dodo.hpp>

struct RandomSourceMeta :
    public dodo::process::ComponentMeta
{
    using nOutPorts = std::integral_constant<unsigned, 1>;
    enum OutNames{Out0};
};

struct SplitterMeta :
    public dodo::process::ComponentMeta
{
    using nInPorts = std::integral_constant<unsigned, 1>;
    enum InNames{In0};
    using nOutPorts = std::integral_constant<unsigned, 2>;
    enum OutNames{Out0, Out1};
};

struct TransformMeta :
    public dodo::process::ComponentMeta
{
    using nInPorts = std::integral_constant<unsigned, 1>;
    enum InNames{In0};
    using nOutPorts = std::integral_constant<unsigned, 1>;
    enum OutNames{Out0};
};

struct PrintMeta :
    public dodo::process::ComponentMeta
{
    using nInPorts = std::integral_constant<unsigned, 1>;
    enum InNames{In0};
};
