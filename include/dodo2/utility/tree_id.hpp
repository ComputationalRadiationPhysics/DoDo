#pragma once

#include <algorithm>

#include <boost/fusion/include/adapt_struct.hpp>
#include "dodo2/types/StringLike.hpp"

namespace dodo
{
namespace utility
{

    class TreeID
    {
    public:
        unsigned children { 0 };
        types::StringLike id;

        TreeID( const TreeID & ) = default;

        TreeID( TreeID && ) = default;

        TreeID & operator=( const TreeID & ) = default;

        TreeID & operator=( TreeID && ) = default;

        ~TreeID( ) = default;


        TreeID( ) :
            id { std::to_string( children ) }
        { }


        TreeID( std::string i ) :
            id { i }
        { }

        TreeID
        genChildID( )
        {
            return { id.value + "." + std::to_string( children++ ) };
        }


        size_t
        getLevel( ) const
        {
            return std::count(
                id.value.begin( ),
                id.value.end( ),
                '.'
            );
        }


        types::StringLike
        get( ) const
        {
            return id.value;
        }

        TreeID
        getParentID( ) const
        {
            auto lastDot = id.value.rfind( '.' );
            if( lastDot == std::string::npos )
            {
                throw std::runtime_error( "Root has no parent node" );
            }
            return { id.value.substr( 0, lastDot ) };
        }



        friend
        std::ostream &
        operator<<(
            std::ostream &stream,
            const TreeID &i
        )
        {
            return stream << i.id;
        }


        friend
        bool
        operator<(
            const TreeID &lhs,
            const TreeID &rhs
        )
        {
            return lhs.id < rhs.id;
        }

        friend
        bool
        operator==(
            const TreeID &lhs,
            const TreeID &rhs
        )
        {
            return lhs.id == rhs.id;
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

BOOST_FUSION_ADAPT_STRUCT(
    dodo::utility::TreeID,
    id
)

