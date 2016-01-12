#pragma once

namespace dodo
{
namespace utility
{



class TreeID
{
    size_t children = 0;
    const std::string id;

public:

    TreeID( ) :
        id(std::to_string(children))
    {}

    TreeID(std::string i) :
        id(i)
    {}

    TreeID
    genChildID()
    {
        return TreeID(id + "." + std::to_string(children++));
    }

    size_t
    getLevel() const
    {
        return std::count(id.begin(), id.end(), '.');
    }

    std::string
    get() const
    {
        return id;
    }


};

struct TreeIDLess :
    public std::binary_function<
        const TreeID,
        const TreeID,
        bool
    >
{
    bool operator()(
        const TreeID a,
        const TreeID b
        ) const
    {
        return a.get() < b.get();
    }
};





} /* utility */
} /* dodo */
