#pragma once

#include <memory>
#include <cstddef> /* size_t */
#include <tuple>
#include <vector>
#include <stack>

#include <boost/any.hpp>

#include <chunky/utility/tuple_index.hpp>

namespace chunky{
namespace graph{

template<typename... AttributeTypes>
class AttributeMapStore
{
    using AttributeTypesTuple = std::tuple<AttributeTypes...>;
    using This_T              = AttributeMapStore<AttributeTypes...>;
    class AttributeHandle;

public:
    enum { AttributeCount = std::tuple_size<AttributeTypesTuple>::value };
    using Handle  = std::shared_ptr<AttributeHandle>;
    using type    = This_T;
    using MapID   = size_t;
    using EntryID = size_t;

private:
    std::vector<std::vector<boost::any>> maps;
    using EntryIDContainer = std::stack<EntryID>;
    std::vector<EntryIDContainer> unusedEntryIDs;
    std::shared_ptr<type> selfPtr;

    AttributeMapStore();
    void       removeEntry(MapID mapID, EntryID entryID);
    EntryID    placeCopy(MapID mapID, boost::any entry);

public:
    static std::shared_ptr<This_T> createInstance();

    template<typename T>
    Handle     addEntry(T t);

    template<typename T>
    Handle     addEntry(MapID mapID, T t);

    template<typename T>
    constexpr MapID getMapID() const;

    template<typename T>
    T          getEntry(EntryID entryID) const;
    boost::any getEntry(MapID mapID, EntryID entryID) const;
    boost::any getEntry(Handle handle) const;

};


template <typename... AttributeTypes>
template <typename T>
constexpr auto AttributeMapStore<AttributeTypes...>::getMapID() const
-> AttributeMapStore<AttributeTypes...>::MapID
{
    constexpr auto tupleIndex = utility::tuple_index<AttributeTypesTuple, T>::value;
    static_assert(tupleIndex >= 0 &&
            static_cast<size_t>(tupleIndex) < std::tuple_size<AttributeTypesTuple>::value,
            "No map exists for type");
    return static_cast<MapID>(tupleIndex);
}


template<typename... AttributeTypes>
auto AttributeMapStore<AttributeTypes...>::removeEntry(MapID mapID, EntryID entryID)
-> void
{
    unusedEntryIDs[mapID].push(entryID);
}


template<typename... AttributeTypes>
auto AttributeMapStore<AttributeTypes...>::createInstance()
-> std::shared_ptr<AttributeMapStore<AttributeTypes...>>
{
    auto* raw_ptr = new This_T;
    auto p = std::shared_ptr<This_T>(raw_ptr);
    p->selfPtr = p;
    return p;
}


template<typename... AttributeTypes>
template<typename T>
auto AttributeMapStore<AttributeTypes...>::addEntry(T t)
-> std::shared_ptr<typename AttributeMapStore<AttributeTypes...>::AttributeHandle>
{
    MapID mapID        = getMapID<T>();
    return addEntry(mapID, t);
}


template<typename... AttributeTypes>
template<typename T>
auto AttributeMapStore<AttributeTypes...>::addEntry(MapID mapID, T t)
-> std::shared_ptr<typename AttributeMapStore<AttributeTypes...>::AttributeHandle>
{
    EntryID entryID    = placeCopy(mapID, boost::any(t));
    AttributeHandle* p = new AttributeHandle(mapID, entryID, selfPtr);
    return Handle(p);
}


template<typename... AttributeTypes>
auto AttributeMapStore<AttributeTypes...>::placeCopy(MapID mapID, boost::any entry)
-> typename AttributeMapStore<AttributeTypes...>::EntryID
{
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
auto AttributeMapStore<AttributeTypes...>::getEntry(EntryID entryID) const
-> T
{
    std::cout << "calling MapID from getEntry" << std::endl;
    MapID mapID = getMapID<T>();
    return boost::any_cast<T>(maps[mapID][entryID]);
}


template<typename... AttributeTypes>
auto AttributeMapStore<AttributeTypes...>::getEntry(MapID mapID, EntryID entryID) const
-> boost::any
{
    return maps[mapID][entryID];
}

template<typename... AttributeTypes>
auto AttributeMapStore<AttributeTypes...>::getEntry(std::shared_ptr<AttributeHandle> handle) const
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
    MapID mapID;
    EntryID entryID;

private:
    using StoreType = AttributeMapStore<AttributeTypes...>;
    std::shared_ptr<StoreType> attributeMapStore;

    template<typename T>
    friend std::shared_ptr<AttributeHandle> StoreType::addEntry(MapID mapID, T t);

    AttributeHandle(MapID pMapID, EntryID pEntryID, std::shared_ptr<StoreType> p)
    :   mapID(pMapID),
        entryID(pEntryID),
        attributeMapStore(p)
    {}
public:
    AttributeHandle(const AttributeHandle&) = delete;
    AttributeHandle& operator=(const AttributeHandle&) = delete;


    ~AttributeHandle()
    {
        attributeMapStore->removeEntry(mapID, entryID);
    }
};


} /* graph */
} /* chunky */
