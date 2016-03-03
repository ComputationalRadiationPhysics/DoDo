#pragma once

namespace dodo
{
namespace hardware
{
namespace attributes
{

struct Memorysize
{
    size_t value;

    Memorysize
    merge( const Memorysize& other )
    {
        return {value + other.value};
    }
};

}
}
}
