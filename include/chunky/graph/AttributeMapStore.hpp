#pragma once

#include <memory>
#include <utility>
#include <tuple>

#include <chunky/utility/tuple_index.hpp>
#include <chunky/graph/BGL.hpp>



namespace chunky{
namespace graph{

template<typename... AttributeTypes>
class AttributeMapStore
{
    using AttributeTypesTuple = std::tuple<AttributeTypes...>;
    using AttributeMapStore_T = AttributeMapStore<AttributeTypes...>;
    using type = AttributeMapStore_T;

    class AttributeHandle;

public:
    using MapID   = size_t;
    using EntryID = size_t;

private:
    std::vector<std::vector<boost::any>> maps;
    using EntryIDContainer = std::stack<EntryID>;
    std::vector<EntryIDContainer> unusedEntryIDs;
    std::shared_ptr<AttributeMapStore_T> selfPtr;

    AttributeMapStore();

public:
    class ChunkyGraphProperty;

    static std::shared_ptr<AttributeMapStore_T> createInstance();
    bool removeEntry(MapID mapID, EntryID entryID);
    boost::any getEntry(MapID mapID, EntryID entryID);
    EntryID placeCopy(MapID mapID, boost::any entry);

    // Experimental
    template<typename T>
    T getEntry(EntryID entryID);

    template<typename T>
    std::shared_ptr<AttributeHandle> addEntry(T t);

    template<typename T>
    MapID getMapID()
    {
        auto tupleIndex = utility::tuple_index<AttributeTypesTuple, T>::value;
        assert(tupleIndex >= 0 && tupleIndex < std::tuple_size<AttributeTypesTuple>::value);
        return static_cast<MapID>(tupleIndex);
    }
};


template<typename... AttributeTypes>
auto AttributeMapStore<AttributeTypes...>::createInstance()
-> std::shared_ptr<typename AttributeMapStore<AttributeTypes...>::AttributeMapStore_T>
{
    auto* raw_ptr = new AttributeMapStore_T;
    auto p = std::shared_ptr<AttributeMapStore_T>(raw_ptr);
    p->selfPtr=p;
    return p;
}

template<typename... AttributeTypes>
template<typename T>
auto AttributeMapStore<AttributeTypes...>::addEntry(T t)
-> std::shared_ptr<typename AttributeMapStore<AttributeTypes...>::AttributeHandle>
{
    MapID mapID = getMapID<T>();
    EntryID entryID = placeCopy(mapID, boost::any(t));
    return std::make_shared<AttributeHandle>(mapID, entryID, selfPtr);
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
        EntryID position = unusedEntryIDs[mapID].top();
        unusedEntryIDs[mapID].pop();
        maps[mapID][position] = entry;
        return position;
    }
}

template<typename... AttributeTypes>
template<typename T>
T AttributeMapStore<AttributeTypes...>::getEntry(EntryID entryID)
{
    MapID mapID = getMapID<T>();
    return boost::any_cast<T>(maps[mapID][entryID]);
}

template<typename... AttributeTypes>
bool AttributeMapStore<AttributeTypes...>::removeEntry(MapID mapID, EntryID entryID)
{
    unusedEntryIDs[mapID].push(entryID);
    return true;
}

template<typename... AttributeTypes>
boost::any AttributeMapStore<AttributeTypes...>::getEntry(MapID mapID, EntryID entryID)
{
    return maps[mapID][entryID];
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
class AttributeMapStore<AttributeTypes...>::ChunkyGraphProperty
{
    std::shared_ptr<AttributeMapStore_T> attributeMapStore;
    std::vector<std::shared_ptr<AttributeHandle> > handles;

    public:
    ChunkyGraphProperty(){}
    ChunkyGraphProperty(std::shared_ptr<AttributeMapStore_T> p) : attributeMapStore(p) {}

    template<typename... Attributes>
    ChunkyGraphProperty(std::shared_ptr<AttributeMapStore_T> p, Attributes... attributes)
    : attributeMapStore(p)
    {
        addEntry(attributes...);
    }

    ChunkyGraphProperty clone()
    {
        ChunkyGraphProperty cloned(attributeMapStore);

        for(auto handle : handles)
        {
            auto mapID = handle->mapID;
            auto entryID = handle->entryID;
            auto entry = attributeMapStore->getEntry(mapID, entryID);
            auto newEntryID = attributeMapStore->placeCopy(mapID, entry);
            cloned.handles.push_back(
                    std::make_shared<AttributeHandle>(mapID, newEntryID, attributeMapStore)
                    );
        }
        return cloned;
    }

    template<typename H, typename... T>
    auto addEntry(H head, T... tail)
    {
        handles.push_back(attributeMapStore->addEntry(head));
        addEntry(tail...);
    }
    auto addEntry(){}

};


template<typename... AttributeTypes>
class AttributeMapStore<AttributeTypes...>::AttributeHandle
{
public:
    MapID mapID;
    EntryID entryID;

private:
    std::shared_ptr<AttributeMapStore_T> attributeMapStore;

public:
    AttributeHandle(const AttributeHandle&) = delete;
    AttributeHandle& operator=(const AttributeHandle&) = delete;

    AttributeHandle(MapID pMapID, EntryID pEntryID, std::shared_ptr<AttributeMapStore_T> p)
    :   mapID(pMapID),
        entryID(pEntryID),
        attributeMapStore(p)
    {}

    ~AttributeHandle()
    {
        attributeMapStore->removeEntry(mapID, entryID);
    }
};


} /* graph */
} /* chunky */

