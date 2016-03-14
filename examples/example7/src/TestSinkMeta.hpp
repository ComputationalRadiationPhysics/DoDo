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
};

}}}}


class TestSinkMeta :
    public dodo::components::meta::InterfaceStatic<TestSinkMeta>
{
public:
    TestSinkMeta()
    {}

};


