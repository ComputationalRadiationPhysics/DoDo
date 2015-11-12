#pragma once

#include <memory>
#include <vector>

#include <chunky/graph/AttributeMapStore.hpp>

namespace chunky{
namespace graph{

template<typename... AttributeTypes>
class Property
{
    using StoreType = AttributeMapStore<AttributeTypes...>;
    using Handle = typename StoreType::Handle;
    std::shared_ptr<StoreType> attributeMapStore;
    std::vector<Handle> handles;

public:
    Property()
    :   handles(StoreType::AttributeCount, nullptr)
    {}

    Property(std::shared_ptr<StoreType> p)
    :   attributeMapStore(p)
      , handles(StoreType::AttributeCount, nullptr)
    {}

    template<typename... Attributes>
    Property(std::shared_ptr<StoreType> p, Attributes... attributes)
    :   attributeMapStore(p)
      , handles(StoreType::AttributeCount, nullptr)
    {
        setEntry(attributes...);
    }

    template<typename H, typename... T>
    auto setEntry(H head, T... tail)
    {
        Handle newHandle = attributeMapStore->addEntry(head);
        handles[newHandle->mapID] = newHandle;
        setEntry(tail...);
    }
    auto setEntry(){}

    Property clone()
    {
        Property cloned(attributeMapStore);

        for(auto handle : handles)
        {
            if(handle != nullptr)
            {
                auto entry = attributeMapStore->getEntry(handle);
                auto newHandle = attributeMapStore->addEntry(handle->mapID, entry);
                cloned.handles[newHandle->mapID] = newHandle;
            }
        }
        return cloned;
    }
};

} /* graph */
} /* chunky */
