#pragma once

namespace dodo
{
namespace physical
{
namespace attributes
{
    struct Bandwidth
    {
        size_t value;

        size_t dynamicValue(size_t);

        Bandwidth merge(const Bandwidth& b) const;

        auto toString() const
        {
            // return value;
            return std::to_string(value);
        }
    };

    size_t Bandwidth::dynamicValue(size_t) {
        return value;
    }

    Bandwidth Bandwidth::merge(const Bandwidth& b) const {
        return { std::min(value, b.value) };
    }

}
}
}
