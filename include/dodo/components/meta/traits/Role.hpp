#pragma once

namespace dodo
{
    namespace components
    {
        namespace meta
        {
            namespace traits
            {

                enum class Roles
                {
                    Undefined, Generator, Filter
                };

                template<typename T_Derived>
                struct Role
                {
                    using value = boost::mpl::vector<
                        decltype(Roles::Undefined)
                    >;
                };


            }
        }
    }
}
