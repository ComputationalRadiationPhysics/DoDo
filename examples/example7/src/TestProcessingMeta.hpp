#pragma once


#include <dodo.hpp>


class TestProcessingMeta;

namespace dodo { namespace components { namespace meta { namespace traits {

    using namespace boost;

    template< >
    struct InPortMap< TestProcessingMeta >
    {
        using value = mpl::map<
            mpl::pair<
                mpl::string< 'in1' >,
                mpl::int_< 1 >
            >,
            mpl::pair<
                mpl::string< 'in2' >,
                mpl::int_< 1 >
            >
        >;

        using test = mpl::map<
            mpl::pair<
                int,
                int
            >
        >;


        static
        auto init( ) -> std::map<
            std::string,
            int
        >
        {
            return {
                {
                    "in1",
                    1
                },
                {
                    "in2",
                    2
                }
            };
        }
    };

    template< >
    struct OutPortMap< TestProcessingMeta >
    {
        using value = mpl::map<
            mpl::pair<
                mpl::string< 'out' >,
                mpl::int_< 1 >
            >
        >;
    };

    template< >
    struct Role< TestProcessingMeta >
    {
        using value = mpl::vector<
            decltype( Roles::Filter )
        >;
    };

} // namespace traits
} // namespace meta
} // namespace components
} // namespace dodo


class TestProcessingMeta :
    public dodo::components::meta::InterfaceStatic< TestProcessingMeta >
{
public:
    TestProcessingMeta( )
    { }

};

