#pragma once

#include <memory>

#include <chunky/graph/Property.hpp>
#include <chunky/graph/AttributeMapStore.hpp>
#include <chunky/graph/BGL.hpp>

namespace chunky{
namespace graph{


template<typename... AttributeTypes>
class AttributeGraph : public BGL<Property<AttributeTypes...>>
{
    using StoreType = AttributeMapStore<AttributeTypes...>;
    std::shared_ptr<StoreType> attributeMapStore;

public:
    using PropertyType = Property<AttributeTypes...>;

public:
    AttributeGraph()
      :  attributeMapStore(StoreType::createInstance())
    {}

    template<typename... T>
    PropertyType createProperty(T... attributes){
        return PropertyType(attributeMapStore, attributes...);
    }

};



} /* graph */
} /* chunky */
