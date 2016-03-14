#pragma once

#include <dodo.hpp>


class TestSourceMeta;

namespace dodo {
namespace components {
namespace meta {
namespace traits
{

using namespace boost;

template<>
struct OutPortMap<TestSourceMeta>
{
    using value = mpl::map<
        mpl::pair<mpl::string<'out'>, mpl::int_<1>>
    >;
};


}}}}

class TestSourceMeta :
    public dodo::components::meta::InterfaceStatic<TestSourceMeta>
{
public:
    TestSourceMeta()
    { }

};


