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

        size_t dynamicValue(size_t chunksize)
        {
                        
            return value;
        }

        Bandwidth merge(const Bandwidth& b) const
        {
            return { std::min(value, b.value) };
        }

        auto toString() const
        {
            // return value;
            return std::to_string(value);
        }
    };

}
}
}
