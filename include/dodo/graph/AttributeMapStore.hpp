#pragma once

#include <memory>
#include <cstddef> /* size_t */
#include <tuple>
#include <vector>
#include <stack>


#include <boost/any.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>

#include <dodo/utility/tuple_index.hpp>

namespace dodo{
namespace graph{

template<typename... AttributeTypes>
class AttributeMapStore :
    public std::enable_shared_from_this<AttributeMapStore<AttributeTypes...>>
{
    using This_T              = AttributeMapStore<AttributeTypes...>;
    class AttributeHandle;

public:
    using AttributeTypesTuple = std::tuple<AttributeTypes...>;
    enum { AttributeCount = std::tuple_size<AttributeTypesTuple>::value };
    using Handle  = std::shared_ptr<AttributeHandle>;
    using type    = This_T;
    using MapID   = size_t;
    using EntryID = size_t;

//private:
    std::vector<std::vector<boost::any>> maps;
    using EntryIDContainer = std::stack<EntryID>;
    std::vector<EntryIDContainer> unusedEntryIDs;

    AttributeMapStore();
    void       removeEntry(MapID mapID, EntryID entryID);
    EntryID    placeCopy(MapID mapID, boost::any entry);

public:
    template<typename T>
    Handle     addEntry(T t);

    template<typename T>
    Handle     addEntry(MapID mapID, T t);

    template<typename T>
    T          getEntry(EntryID entryID) const;
    boost::any getEntry(MapID mapID, EntryID entryID) const;
    boost::any getEntry(Handle handle) const;

};

template <typename T, typename... AttributeTypes>
constexpr auto getMapID(std::weak_ptr<AttributeMapStore<AttributeTypes...>>)
-> typename AttributeMapStore<AttributeTypes...>::MapID
{
    using StoreType = AttributeMapStore<AttributeTypes...>;
    using AttributeTypesTuple = typename StoreType::AttributeTypesTuple;
    constexpr auto tupleIndex = utility::tuple_index<AttributeTypesTuple, T>::value;
    static_assert(tupleIndex >= 0 &&
        static_cast<size_t>(tupleIndex) < std::tuple_size<AttributeTypesTuple>::value,
        "No map exists for type");
    return static_cast<typename StoreType::MapID>(tupleIndex);
}

template<typename NewType, typename... OldTypes>
auto
clone_and_strip_to(
    std::shared_ptr<AttributeMapStore<OldTypes...>> oldStore
)
-> std::shared_ptr<AttributeMapStore<NewType>>
{
    auto newStore = std::make_shared<AttributeMapStore<NewType>>();
    auto oldTypeIndex = getMapID<NewType>(oldStore);
    newStore->maps[0] = oldStore->maps[oldTypeIndex];
    return newStore;
}


template<typename T, typename... V>
auto
clone_and_combine(
    std::shared_ptr<AttributeMapStore<T>> old1,
    std::shared_ptr<AttributeMapStore<V...>> oldStore
)
-> std::shared_ptr<AttributeMapStore<T, V...>>
{
    auto newStore = std::make_shared<AttributeMapStore<T, V...>>();
    newStore->maps[getMapID<T>(newStore)] = old1.maps[0];

    using AttributeTuple = boost::mpl::list<V...>;
    boost::mpl::for_each<AttributeTuple>(
        [&newStore, &oldStore](auto U)
            {
                using U_t = decltype(U);
                newStore->maps[getMapID<U_t>(newStore)] = oldStore->maps[getMapID<U_t>(oldStore)];
            }
        );
}


template <typename T, typename... Attrs>
constexpr
bool
hasType(const std::weak_ptr<AttributeMapStore<Attrs...>>)
{
    using AttributeTypesTuple = std::tuple<Attrs...>;
    constexpr auto tupleIndex = utility::tuple_index<AttributeTypesTuple, T>::value;
    return tupleIndex >= 0;
}


template<typename... AttributeTypes>
auto
AttributeMapStore<AttributeTypes...>::removeEntry(MapID mapID, EntryID entryID)
-> void
{
    unusedEntryIDs[mapID].push(entryID);
}


template<typename... AttributeTypes>
template<typename T>
auto AttributeMapStore<AttributeTypes...>::addEntry(T t)
-> std::shared_ptr<typename AttributeMapStore<AttributeTypes...>::AttributeHandle>
{
    MapID mapID        = getMapID<T>(std::weak_ptr<AttributeMapStore>(this->shared_from_this()));
    return addEntry(mapID, t);
}


template<typename... AttributeTypes>
template<typename T>
auto AttributeMapStore<AttributeTypes...>::addEntry(MapID mapID, T t)
-> std::shared_ptr<typename AttributeMapStore<AttributeTypes...>::AttributeHandle>
{
    EntryID entryID    = placeCopy(mapID, boost::any(t));
    return AttributeHandle::create(
        mapID,
        entryID,
        this->shared_from_this()
    );
}


template<typename... AttributeTypes>
auto
AttributeMapStore<AttributeTypes...>::placeCopy(MapID mapID, boost::any entry)
-> typename AttributeMapStore<AttributeTypes...>::EntryID
{
    auto x = unusedEntryIDs;
    if(unusedEntryIDs.at(mapID).empty())
    {
        maps[mapID].emplace_back(entry);
        return maps[mapID].size()-1;
    } else{
        EntryID newEntryID = unusedEntryIDs[mapID].top();
        unusedEntryIDs[mapID].pop();
        maps[mapID][newEntryID] = entry;
        return newEntryID;
    }
}


template<typename... AttributeTypes>
template<typename T>
auto
AttributeMapStore<AttributeTypes...>::getEntry(EntryID entryID) const
-> T
{
    MapID mapID = getMapID<T>(this->shared_from_this());
    return boost::any_cast<T>(maps[mapID][entryID]);
}


template<typename... AttributeTypes>
auto
AttributeMapStore<AttributeTypes...>::getEntry(
    MapID mapID,
    EntryID entryID
) const
-> boost::any
{
    return maps[mapID][entryID];
}

template<typename... AttributeTypes>
auto
AttributeMapStore<AttributeTypes...>::getEntry(
    std::shared_ptr<AttributeHandle> handle
) const
-> boost::any
{
    return maps[handle->mapID][handle->entryID];
}


template<typename... AttributeTypes>
AttributeMapStore<AttributeTypes...>::AttributeMapStore()
{
    const size_t n = std::tuple_size<AttributeTypesTuple>::value;

    for(size_t i = 0 ; i<n ; ++i)
    {
        maps.emplace_back(std::vector<boost::any>());
    }

    for(size_t i = 0 ; i<n ; ++i)
    {
        unusedEntryIDs.emplace_back(EntryIDContainer());
    }

    maps.resize(n);
    unusedEntryIDs.resize(n);
}


template<typename... AttributeTypes>
class AttributeMapStore<AttributeTypes...>::AttributeHandle
{

public:
    const MapID mapID;
    const EntryID entryID;

private:
    using StoreType = AttributeMapStore<AttributeTypes...>;
    std::weak_ptr<StoreType> attributeMapStore;
    class HiddenType{};

public:
    AttributeHandle(const AttributeHandle&) = delete;
    AttributeHandle(const AttributeHandle&&) = delete;
    AttributeHandle& operator=(const AttributeHandle&) = delete;
    AttributeHandle& operator=(const AttributeHandle&&) = delete;

    explicit AttributeHandle(MapID pMapID, EntryID pEntryID, std::weak_ptr<StoreType> p, HiddenType)
    :   mapID(pMapID),
        entryID(pEntryID),
        attributeMapStore(p)
    {}

    static
    std::unique_ptr<AttributeHandle>
    create(MapID pMapID, EntryID pEntryID, std::weak_ptr<StoreType> p)
    {
        return std::make_unique<AttributeHandle>(pMapID, pEntryID, p, HiddenType());
    }

    ~AttributeHandle()
    {
        if( !attributeMapStore.expired() )
            attributeMapStore.lock()->removeEntry(mapID, entryID);
    }
};


} /* graph */
} /* dodo */
