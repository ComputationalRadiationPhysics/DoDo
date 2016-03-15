#pragma once

#include <dodo.hpp>

class TestProcessingMeta;

namespace dodo {
namespace components {
namespace meta {
namespace traits {

using namespace boost;

template<>
struct InPortMap<TestProcessingMeta>
{
    using value = mpl::map<
        mpl::pair<mpl::string<'in1'>, mpl::int_<1>>,
        mpl::pair<mpl::string<'in2'>, mpl::int_<1>>
    >;
};

template<>
struct OutPortMap<TestProcessingMeta>
{
    using value = mpl::map<
        mpl::pair<mpl::string<'out'>, mpl::int_<1>>
    >;
};

template<>
struct Role<TestProcessingMeta>
{
    using value = mpl::vector<
        decltype(Roles::Filter)
    >;
};

}}}}



class TestProcessingMeta :
    public dodo::components::meta::InterfaceStatic<TestProcessingMeta>
{
public:
    TestProcessingMeta()
    {}

};

