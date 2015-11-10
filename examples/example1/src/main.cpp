#include <iostream>
#include <vector>
#include <utility>
#include <memory>
#include <tuple>

#include <boost/graph/adjacency_list.hpp>

#include <chunky/utility.hpp>
#include <chunky/BGL.hpp>
#include <chunky/physical/attributes.hpp>


namespace chunky{

template<typename... MapTypes>
class PropertyMapStore
{
    using MapTypesTuple = std::tuple<MapTypes...>;
    using StoreType = PropertyMapStore<MapTypes...>;
    using type = StoreType;
    std::vector<std::vector<boost::any>> maps;
    std::vector<std::stack<size_t>> unusedEntryIDs;
    std::shared_ptr<StoreType> selfPtr;

    
public:
    class PropertyHandle;

private:
    PropertyMapStore(){
        const size_t n = std::tuple_size<MapTypesTuple>::value;
                
        for(size_t i = 0 ; i<n ; ++i){
            maps.emplace_back(std::vector<boost::any>());

        }
        for(size_t i = 0 ; i<n ; ++i){
            unusedEntryIDs.emplace_back(std::stack<size_t>());
        }

        maps.resize(n);
        unusedEntryIDs.resize(n);
    };


public:
    static std::shared_ptr<StoreType> createInstance(){
        auto* raw_ptr = new StoreType;
        auto p = std::shared_ptr<StoreType>(raw_ptr);
        p->selfPtr=p;
        return p;
    }


    bool removeEntry(size_t mapID, size_t entryID)
    {
        unusedEntryIDs[mapID].push(entryID);
        return true;
    }

    boost::any getEntry(size_t mapID, size_t entryID)
    {
        return maps[mapID][entryID];
    }

    //experimental
    template<typename T>
    T getEntry(size_t entryID)
    {
        size_t mapID = static_cast<size_t>(utility::tuple_index<MapTypesTuple, T>::value);
        return boost::any_cast<T>(maps[mapID][entryID]);
    }

    size_t placeCopy(size_t mapID, boost::any entry)
    {
        if(unusedEntryIDs.at(mapID).empty())
        {
            maps[mapID].emplace_back(entry); 
            return maps[mapID].size()-1;
        }else{
            size_t position = unusedEntryIDs[mapID].top();
            unusedEntryIDs[mapID].pop();
            maps[mapID][position] = entry;
            return position;
        }
    }




    class PropertyHandle
    {
        // don't allow copying or assignment
        PropertyHandle(const PropertyHandle&) = delete;
        PropertyHandle& operator=(const PropertyHandle&) = delete;

        public:
        size_t first;
        size_t second;

        private:
        std::shared_ptr<StoreType> propertyMapStore;

        public:
        PropertyHandle(size_t pfirst, size_t psecond, std::shared_ptr<StoreType> p) :
            first(pfirst),
            second(psecond),
            propertyMapStore(p)
        {}

        ~PropertyHandle()
        {
            propertyMapStore->removeEntry(first, second);
        }
    };


    //template<typename T>
    //auto addEntry(T t)
    //-> decltype(std::make_shared<typename StoreType::PropertyHandle>()) // C++14: legacy
    //
    template<typename T>
    std::shared_ptr<typename StoreType::PropertyHandle> addEntry(T t)
    {
         size_t mapID = static_cast<size_t>(utility::tuple_index<MapTypesTuple, T>::value);
         size_t entryID = placeCopy(mapID, boost::any(t));
         return std::make_shared<typename StoreType::PropertyHandle>(mapID, entryID, selfPtr);
    }


    class ComputeVertexProperty {
        std::shared_ptr<StoreType> propertyMapStore;
        size_t id;
        std::vector<std::shared_ptr<PropertyHandle> > handles;

        public:
        ComputeVertexProperty(){
            id = utility::UniqueID::get();
        }

        ComputeVertexProperty(std::shared_ptr<StoreType> p) :
            propertyMapStore(p)
        {
            id = utility::UniqueID::get();
        }

        ComputeVertexProperty(std::shared_ptr<StoreType> p, size_t id) :
            propertyMapStore(p),
            id(id)
        {}

        ComputeVertexProperty clone()
        {
            ComputeVertexProperty cloned(propertyMapStore, utility::UniqueID::get());
            cloned.handles.clear();

            for(auto handle_ptr : handles){
                std::shared_ptr<PropertyHandle> handle = handle_ptr;//.get();
                auto mapID = handle->first;
                auto entryID = handle->second;
                auto entry = propertyMapStore->getEntry(mapID, entryID);
                auto newEntryID = propertyMapStore->placeCopy(mapID, entry);
                cloned.handles.push_back(
                        std::make_shared<PropertyHandle>(mapID, newEntryID, propertyMapStore)
                        );
            }
            return cloned;
        }

        template<typename T>
        auto addEntry(T t)
        {
            std::shared_ptr<PropertyHandle> p = propertyMapStore->addEntry(t);                       
            handles.push_back(propertyMapStore->addEntry(t));
            return p;
        }
    
    };

    class ComputeVertexPropertyGenerator{
        
        template<typename T>
        void generateProperty(ComputeVertexProperty& cvp, T t){
            cvp.addEntry(t);
        }

        template<typename T, typename... Properties>
        void generateProperty(ComputeVertexProperty& cvp, T t, Properties... properties){
            cvp.addEntry(t);
            generateProperty(cvp, properties...);

        }
        std::shared_ptr<StoreType> propertyMapStore;

    public:

        ComputeVertexPropertyGenerator(std::shared_ptr<StoreType> pms) : propertyMapStore(pms) {};

        template<typename... Properties>
        ComputeVertexProperty generate(Properties... properties){
            ComputeVertexProperty cvp(propertyMapStore);
            generateProperty(cvp, properties...);
            return cvp;
        }
        
    };

};

}

int main()
{
    using namespace chunky;


    using PMS = PropertyMapStore<physical::attributes::EnergyLevel>;
    using ComputeVertexProperty = typename PMS::ComputeVertexProperty;
    using ComputeGraph = BGL<ComputeVertexProperty>;
    ComputeGraph graph;
    std::shared_ptr<PMS> pms = PMS::createInstance();

    ComputeVertexProperty node(pms);
    node.addEntry(physical::attributes::EnergyLevel{3});

    graph.addVertex(node.clone());


    return EXIT_SUCCESS;

}
