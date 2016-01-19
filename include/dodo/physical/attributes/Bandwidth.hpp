#pragma once

namespace dodo
{
namespace physical
{
namespace attributes
{
    struct Bandwidth
    {
        int value;

        Bandwidth merge(const Bandwidth& b)
        {
            return { std::min(value, b.value) };
        }
    };

}
}
}
