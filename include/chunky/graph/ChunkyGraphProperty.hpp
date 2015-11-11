#pragma once

#include <memory>
#include <vector>

namespace chunky{
namespace graph{

template<typename AttributeMapStore_T>
class ChunkyGraphProperty
{
    using AttributeMapStore = AttributeMapStore_T;
    std::shared_ptr<AttributeMapStore> attributeMapStore;
    std::vector<typename AttributeMapStore::Handle> handles;

public:
    ChunkyGraphProperty(){}
    ChunkyGraphProperty(std::shared_ptr<AttributeMapStore> p) : attributeMapStore(p) {}

    template<typename... Attributes>
    ChunkyGraphProperty(std::shared_ptr<AttributeMapStore> p, Attributes... attributes)
    : attributeMapStore(p)
    {
        addEntry(attributes...);
    }

    template<typename H, typename... T>
    auto addEntry(H head, T... tail)
    {
        handles.push_back(attributeMapStore->addEntry(head));
        addEntry(tail...);
    }
    auto addEntry(){}

    ChunkyGraphProperty clone()
    {
        ChunkyGraphProperty cloned(attributeMapStore);

        for(auto handle : handles)
        {
            auto newHandle = attributeMapStore->addEntry(
                attributeMapStore->getEntry(handle)
                );
            cloned.handles.push_back(newHandle);
        }
        return cloned;
    }

};

} /* graph */
} /* chunky */
