#pragma once

#include <map>
#include <string>
#include <memory>

#include "types.hpp"
#include "detail/MetadataNetworkImpl.hpp"


namespace dodo
{
namespace components
{

class MetadataNetwork
{
    std::shared_ptr<detail::MetadataNetworkImpl> impl;
public:
    MetadataNetwork() :
        impl( std::make_shared<detail::MetadataNetworkImpl>() )
    { }

    template <typename T_Component>
    auto addComponent()
    {
        return impl->addComponent<T_Component>();
    }

    void addDependency(
        std::pair<types::ResourceID, types::PortKey> portA,
        std::pair<types::ResourceID, types::PortKey> portB
    ){
        return impl->addDependency(portA, portB);
    }

};

}
}

