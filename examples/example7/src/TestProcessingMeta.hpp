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
            mpl::pair<mpl::string<'in'>, mpl::int_<1>>
        >;
};

template<>
struct OutPortMap<TestProcessingMeta>
{
        using value = mpl::map<
            mpl::pair<mpl::string<'out'>, mpl::int_<1>>
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

