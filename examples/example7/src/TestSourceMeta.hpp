#pragma once

#include <dodo.hpp>

class TestSourceMeta;

namespace dodo
{
    namespace components
    {
        namespace meta
        {
            namespace traits
            {

                namespace bmpl = boost::mpl;

                template<>
                struct OutPortMap<TestSourceMeta>
                {
                    using value = bmpl::map<
                        bmpl::pair<bmpl::string<'out'>, bmpl::int_<1>>
                    >;
                };

                template<>
                struct Role<TestSourceMeta>
                {
                    using value = bmpl::vector<
                        decltype(Roles::Generator)
                    >;
                };


            }
        }
    }
}

class TestSourceMeta :
    public dodo::components::meta::InterfaceStatic<TestSourceMeta>
{
public:
    TestSourceMeta()
    {}

};


