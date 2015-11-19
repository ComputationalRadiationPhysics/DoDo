#pragma once

#include <memory>
#include <vector>

#include <boost/hana.hpp>

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

    template<typename T>
    T getEntry(){
        auto mapID = attributeMapStore-> template getMapID<T>();
        return boost::any_cast<T>(attributeMapStore->getEntry(handles[mapID]));
    }

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

    // template parameters that we want to convert to
    template<typename... Attrs>
    void remapHandles(Property<Attrs...> &destination){
        using T_tuple = boost::mpl::list<Attrs...>;
        boost::mpl::for_each<T_tuple>(
                [this, &destination](auto t){
                    using T = decltype(t);
                    if(hasType<T>(attributeMapStore)){
                        //auto mapIDsource = attributeMapStore-> template getMapID<T>();
                        T entry = this->getEntry<T>();
                        destination.setEntry(entry);
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
} /* chunky */
