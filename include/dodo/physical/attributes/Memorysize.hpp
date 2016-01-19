#pragma once

namespace dodo
{
namespace physical
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
