#pragma once


#include <tuple>


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

        OneToNMap(){};
        OneToNMap(
            const std::map<
                T_N,
                T_One
            > & worker2hw
        ) :
            n2one( n2one )
        {
            for( auto & i : n2one )
            {
                one2n[i.second].push_back( i.first );
            }
        }

        OneToNMap(
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

        void
        eraseMapping( T_N n)
        {
            if( n2one.find( n ) != n2one.end( ) )
            {
                const auto & currentOne = n2one.at( n );
                const auto & currentNList = one2n.at( currentOne );
                std::remove(
                    currentNList,
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

    };

} /* utility */
} /* dodo */
