#pragma once

namespace dodo
{
    namespace components
    {
        namespace meta
        {

            /**
             * Defines multiple traits that need to be defined for each component
             *
             * Some might be optional (to be determined later)
             */
            namespace traits
            {

                template<typename T_Derived>
                struct InPortMap
                {
                    using value = boost::mpl::map<>;
                    static auto init()
                    {
                        std::cout << "in default portmap operator" << std::endl;
                        return std::map<int,int>();
                    }
                };


            }
        }
    }
}
