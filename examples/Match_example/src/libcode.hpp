

#include <mach7.hpp>
#include <algorithm>


namespace lib{

struct Base
{
    virtual ~Base() = default;
};

template<typename T_Seq>
struct Arbiter{
    template<template<typename> class T>
    static void apply(Base* b)
    {
        boost::fusion::for_each(
            T_Seq{},
            [&](auto i)
            {
                using I = decltype(i);
                if(I* a = dynamic_cast<I*>(b)){ T<I>()(); return; }
            }
        );
    }


    template<
        template< typename > class T,
        typename T_Sequence,
        typename = typename T_Sequence::iterator
    >
    static void apply(
        T_Sequence sequence
    )
    {
        for(auto i : sequence)
        {
            apply<T>(i);
        }
    }

};

template<typename T>
struct StaticName
{
};

}
