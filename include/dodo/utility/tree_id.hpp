#pragma once

namespace dodo
{
namespace utility
{



class TreeID
{
    unsigned children{0};
    const std::string id;

public:

    TreeID( ) :
        id{std::to_string(children)}
    {}

    TreeID(std::string i) :
        id{i}
    {}

    TreeID
    genChildID()
    {
        return {id + "." + std::to_string(children++)};
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

    friend
    std::ostream&
    operator<<(
        std::ostream& stream,
        const TreeID& i
    ){
        return stream << i.id;
    }

    friend
    bool
    operator<(
        const TreeID& lhs,
        const TreeID& rhs
    ){
        return lhs.id < rhs.id;
    }

};

// struct TreeIDLess :
//     public std::binary_function<
//         const TreeID,
//         const TreeID,
//         bool
//     >
// {
//     bool operator()(
//         const TreeID a,
//         const TreeID b
//         ) const
//     {
//         return a.get() < b.get();
//     }
// };





} /* utility */
} /* dodo */
