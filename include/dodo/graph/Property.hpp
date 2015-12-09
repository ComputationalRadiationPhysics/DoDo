#pragma once

#include <memory>
#include <vector>

#include <boost/variant.hpp>

#include <dodo/graph/AttributeMapStore.hpp>

namespace dodo
{
namespace graph
{

template<typename... AttributeTypes>
class Property
{
    using StoreType = AttributeMapStore<AttributeTypes...>;
    using Handle = typename StoreType::Handle;
    std::weak_ptr<StoreType> attributeMapStore;
    std::vector<Handle> handles;

public:
    Property()
      : attributeMapStore()
      , handles(StoreType::AttributeCount, nullptr)
    {}

    Property(std::weak_ptr<StoreType> p)
      : attributeMapStore(p)
      , handles(StoreType::AttributeCount, nullptr)
    {}

    template<typename... Attributes>
    Property(std::weak_ptr<StoreType> p, Attributes... attributes)
      : attributeMapStore(p)
      , handles(StoreType::AttributeCount, nullptr)
    {
        setEntry(attributes...);
    }

    template<typename H, typename... T>
    auto setEntry(H head, T... tail)
    {
        Handle newHandle = attributeMapStore.lock()->addEntry(head);
        handles[newHandle->mapID] = newHandle;
        setEntry(tail...);
    }
    auto setEntry(){}

    template<typename T>
    T getEntry()
    {
        auto mapID = getMapID<T>(attributeMapStore);
        return boost::get<T>(attributeMapStore.lock()->getEntry(handles[mapID]));
    }
    template<typename T>
    bool hasEntry()
    {
        auto mapID = getMapID<T>(attributeMapStore);
        return nullptr != (attributeMapStore.lock()->getEntry(handles[mapID]));
    }

    Property clone()
    {
        Property cloned(attributeMapStore);

        for(auto handle : handles)
        {
            if(handle != nullptr)
            {
                auto entry = attributeMapStore.lock()->getEntry(handle);
                auto newHandle = attributeMapStore.lock()->addEntry(handle->mapID, entry);
                cloned.handles[newHandle->mapID] = newHandle;
            }
        }
        return cloned;
    }

    // template parameters that we want to convert to
    template<typename... Attrs>
    void remapHandles(Property<Attrs...> &destination){
        using T_tuple = boost::mpl::list<Attrs...>;
        boost::mpl::for_each<T_tuple>(
            [this, &destination](auto t)
            {
                using T = decltype(t);
                if( !attributeMapStore.expired() && hasType<T>(attributeMapStore) )
                {
                    if(handles[getMapID<T>(attributeMapStore)] != nullptr)
                    {
                        T entry = this->getEntry<T>();
                        destination.setEntry(entry);
                    }
                }
            }
        );
    }

};

/* template<typename DestinationProperty, typename SourceProperty> */
/* transferHandles(DestinationProperty& dp, const SourceProperty& sp) */
/* { */
/*     boost::mpl::for_each<DestinationProperty::StoreType:: */
/* } */

} /* graph */
} /* dodo */
