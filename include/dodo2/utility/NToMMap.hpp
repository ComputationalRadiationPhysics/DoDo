#pragma once

#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>


namespace dodo
{
namespace utility
{

    template<typename T_N, typename T_M>
    class NToMMap
    {

    public:
        using MapType = boost::bimap<boost::bimaps::multiset_of<T_N>, boost::bimaps::multiset_of<T_M>>;
        MapType nm;




//        explicit OneToNMap() :
//            one2n{},
//            n2one{}
//        {}
//        explicit OneToNMap(
//            const std::map<
//                T_N,
//                T_One
//            > & pn2one
//        ) :
//            one2n{},
//            n2one( pn2one )
//        {
//            for( auto & i : n2one )
//            {
//                one2n[i.second].push_back( i.first );
//            }
//        }

//        explicit OneToNMap(
//            const std::map<
//                T_One,
//                std::vector< T_N >
//            > & pone2n
//        ) :
//            one2n( pone2n ),
//            n2one{}
//        {
//            for( auto & i : one2n )
//            {
//                for( auto & j : i.second )
//                {
//                    n2one.insert(
//                        std::make_pair(
//                            j,
//                            i.first
//                        )
//                    );
//                }
//            }
//        }

//        auto
//        eraseMapping( T_N n )
//        -> void
//        {
//            if( n2one.find( n ) != n2one.end( ) )
//            {
//                const auto & currentOne = n2one.at( n );
//                auto & currentNList = one2n.at( currentOne );

//                std::remove(
//                    currentNList.begin( ),
//                    currentNList.end( ),
//                    n
//                );
//            }
//        }

//        void
//        addMapping( T_One o, T_N n)
//        {
//            one2n[o].push_back(n);
//            n2one[n] = o;
//        }

//        auto
//        addMapping( std::pair<T_One, T_N> p )
//        -> void
//        {
//            T_One o; T_N n;
//            std::tie(o,n) = p;
//            addMapping(o,n);
//        }

//        auto
//        moveNToOne(
//            T_N n,
//            T_One o
//        )
//        -> void
//        {
//            eraseMapping( n );
//            addMapping(
//                o,
//                n
//            );
//        }

    };

} /* utility */
} /* dodo */
