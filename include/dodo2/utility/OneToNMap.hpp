#pragma once


#include <map>
#include <vector>
#include <algorithm>


namespace dodo
{
namespace utility
{

    template<typename T_One, typename T_N>
    class OneToNMap
    {

    public:
        std::map<
            T_One,
            std::vector< T_N >
        > one2n;
        std::map<
            T_N,
            T_One
        > n2one;

        explicit OneToNMap(){};
        explicit OneToNMap(
            const std::map<
                T_N,
                T_One
            > & n2one
        ) :
            n2one( n2one )
        {
            for( auto & i : n2one )
            {
                one2n[i.second].push_back( i.first );
            }
        }

        explicit OneToNMap(
            const std::map<
                T_One,
                std::vector< T_N >
            > & one2n
        ) :
            one2n( one2n )
        {
            for( auto & i : one2n )
            {
                for( auto & j : i.second )
                {
                    n2one.insert(
                        std::make_pair(
                            j,
                            i.first
                        )
                    );
                }
            }
        }

        auto
        eraseMapping( T_N n )
        -> void
        {
            if( n2one.find( n ) != n2one.end( ) )
            {
                const auto & currentOne = n2one.at( n );
                auto & currentNList = one2n.at( currentOne );

                std::remove(
                    currentNList.begin( ),
                    currentNList.end( ),
                    n
                );
            }
        }

        void
        addMapping( T_One o, T_N n)
        {
            one2n[o].push_back(n);
            n2one[n] = o;
        }

        auto
        addMapping( std::pair<T_One, T_N> p )
        -> void
        {
            T_One o; T_N n;
            std::tie(o,n) = p;
            addMapping(o,n);
        }

        auto
        moveNToOne(
            T_N n,
            T_One o
        )
        -> void
        {
            eraseMapping( n );
            addMapping(
                o,
                n
            );
        }

    };

} /* utility */
} /* dodo */
