#include <iostream>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/vector.hpp>

//#include <mach7.hpp>


#include "libcode.hpp"



struct DerivedA : public lib::Base
{

};

struct DerivedB : public lib::Base
{

};


template<typename>
struct PrintName
{};

template<>
struct PrintName<DerivedA>
{
    void operator()()
    {
        std::cout << "DerivedA" << std::endl;
    }
};

template<>
struct PrintName<DerivedB>
{
    void operator()()
    {
        std::cout << "DerivedB" << std::endl;
    }
};


int main( )
{
    std::vector<lib::Base*> b;

    b.push_back(new DerivedA);
    b.push_back(new DerivedB);

    using Seq = boost::fusion::vector<DerivedA, DerivedB>;
    lib::Arbiter<Seq>::apply<PrintName>(b);

    return 0;
}
