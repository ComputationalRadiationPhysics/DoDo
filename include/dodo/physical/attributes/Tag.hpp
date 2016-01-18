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

    Tag merge(const Tag& t){
        assert(t.value == value);
        return t;
    }

};

}
}
}
