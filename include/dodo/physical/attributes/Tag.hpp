#pragma once


namespace dodo
{
namespace physical
{
namespace attributes
{

struct Tag
{
    enum class Tags{ Unknown, Switch, Compute, Memory } value;

    Tag
    merge( const Tag& t )
    {
        if(t.value != value)
            throw std::runtime_error("Cannot merge Elements with different Tags");
        return t;
    }

};

}
}
}
