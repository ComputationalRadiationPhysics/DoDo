#pragma once

namespace dodo
{
    namespace components
    {
        namespace meta
        {
            namespace traits
            {

                template<typename T_Derived>
                struct Complexity
                {
                    using value = boost::mpl::map<>;
                };


            }
        }
    }
}
