#pragma once

#include <string>

#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/pair.hpp>

#include <boost/hana.hpp>

#include <dodo.hpp>

#include <iostream>




class TestSinkMeta;

namespace dodo {
namespace components {
namespace meta {
namespace traits {

using namespace boost;

template<>
struct InPortMap<TestSinkMeta>
{
    using value = mpl::map<
        mpl::pair<mpl::string<'in'>, mpl::int_<1>>
    >;

    static auto init()
    {
        std::map<std::string, int> m;
        boost::fusion::for_each(
            value{},
            [&](auto i)
            {
                using Elem = decltype(i);
                using KeyType = typename Elem::first;
                using ValueType = typename Elem::second;
                m[boost::mpl::c_str<KeyType>::value] = ValueType().value;
            }
        );
        std::cout << "in overloaded portMap" << std::endl;
        return m;
    }
};

template<>
struct Role<TestSinkMeta>
{
    using value = mpl::vector<
        decltype(Roles::Filter)
    >;
};

}}}}


class TestSinkMeta :
    public dodo::components::meta::InterfaceStatic<TestSinkMeta>
{
public:
    TestSinkMeta()
    {}

};


