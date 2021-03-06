#pragma once

#include <algorithm>

#include <boost/fusion/include/adapt_struct.hpp>
#include "dodo/types/StringLike.hpp"

namespace dodo
{
namespace utility
{
    class TreeID
    {
    public:
        std::string id;
        unsigned children { 0 };
        unsigned padding { 0 };

        TreeID( const TreeID & ) = default;

        TreeID( TreeID && ) = default;

        TreeID & operator=( const TreeID & ) = default;

        TreeID & operator=( TreeID && ) = default;

        ~TreeID( ) = default;


        explicit
        TreeID( ) :
            id { std::to_string( 0 ) }
        { }


        explicit
        TreeID( std::string i ) :
            id { i }
        { }

        TreeID
        genChildID( )
        {
            return TreeID( id + "." + std::to_string( children++ ) );
        }


        size_t
        getLevel( ) const
        {
            return static_cast<size_t>(
                std::count(
                    id.begin( ),
                    id.end( ),
                    '.'
                )
            );
        }


        std::string
        get( ) const
        {
            return id;
        }

        TreeID
        getParentID( ) const
        {
            auto lastDot = id.rfind( '.' );
            if( lastDot == std::string::npos )
            {
                throw std::runtime_error( "Root has no parent node" );
            }
            return TreeID( id.substr( 0, lastDot ) );
        }

        auto
        hasIndirectChild( const TreeID& possibleChild )
        const -> bool
        {
            return std::equal(
                possibleChild.id.begin(),
                possibleChild.id.begin() + static_cast<long>(std::min( possibleChild.id.size(), id.size() ) ) ,
                id.begin()
            );
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

//    class TreeID
//    {
//    public:
//        unsigned children { 0 };
//        types::StringLike id;
//
//        TreeID( const TreeID & ) = default;
//
//        TreeID( TreeID && ) = default;
//
//        TreeID & operator=( const TreeID & ) = default;
//
//        TreeID & operator=( TreeID && ) = default;
//
//        ~TreeID( ) = default;
//
//
//        TreeID( ) :
//            id { std::to_string( children ) }
//        { }
//
//
//        TreeID( std::string i ) :
//            id { i }
//        { }
//
//        TreeID
//        genChildID( )
//        {
//            return { id.value + "." + std::to_string( children++ ) };
//        }
//
//
//        size_t
//        getLevel( ) const
//        {
//            return std::count(
//                id.value.begin( ),
//                id.value.end( ),
//                '.'
//            );
//        }
//
//
//        types::StringLike
//        get( ) const
//        {
//            return id.value;
//        }
//
//        TreeID
//        getParentID( ) const
//        {
//            auto lastDot = id.value.rfind( '.' );
//            if( lastDot == std::string::npos )
//            {
//                throw std::runtime_error( "Root has no parent node" );
//            }
//            return { id.value.substr( 0, lastDot ) };
//        }
//
//        auto
//        hasIndirectChild( const TreeID& possibleChild )
//        const -> bool
//        {
//            return std::equal(
//                possibleChild.id.value.begin(),
//                possibleChild.id.value.begin() + std::min( possibleChild.id.size(), id.size() ),
//                id.value.begin()
//            );
//        }
//
//
//
//        friend
//        std::ostream &
//        operator<<(
//            std::ostream &stream,
//            const TreeID &i
//        )
//        {
//            return stream << i.id;
//        }
//
//
//        friend
//        bool
//        operator<(
//            const TreeID &lhs,
//            const TreeID &rhs
//        )
//        {
//            return lhs.id < rhs.id;
//        }
//
//        friend
//        bool
//        operator==(
//            const TreeID &lhs,
//            const TreeID &rhs
//        )
//        {
//            return lhs.id == rhs.id;
//        }
//
//
//    };


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



namespace boost
{
    std::string
    lexical_cast( const dodo::utility::TreeID & );

    std::string
    lexical_cast( const dodo::utility::TreeID & t )
    {
        return t.id;
    }

    template<>
    dodo::utility::TreeID
    lexical_cast( const std::string & s )
    {
        return dodo::utility::TreeID( s );
    }
} /* boost */
//
//BOOST_FUSION_ADAPT_STRUCT(
//    dodo::utility::TreeID,
//    id
//)

