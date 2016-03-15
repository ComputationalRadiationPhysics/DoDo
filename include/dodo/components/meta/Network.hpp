#pragma once


#include <map>
#include <string>
#include <memory>
#include <utility>

#include "../types.hpp"
#include "dodo/components/meta/detail/NetworkImpl.hpp"


namespace dodo
{
    namespace components
    {
        namespace meta
        {

            class Network
            {
                std::shared_ptr<detail::NetworkImpl> impl;
            public:
                Network() :
                    impl(std::make_shared<detail::NetworkImpl>())
                {
                }


                template<typename T_Component>
                auto addComponent()
                {
                    return impl->addComponent<T_Component>();
                }


                void addDependency(
                    std::pair<types::ResourceID, types::Base> portA,
                    std::pair<types::ResourceID, types::Base> portB
                )
                {
                    return impl->addDependency(portA, portB);
                }


                std::weak_ptr<dependency::BGL> extractDependencyGraph()
                {
                    return impl->getDependencies();
                }

            };


        }
    }
}

