#pragma once


#include <dodo2/graph/TreeIDGraph.hpp>
#include <dodo2/graph/InterconnectGraph.hpp>
#include <dodo2/model/hardware/property/VertexType.hpp>
#include <dodo2/utility/tuple_index.hpp>
#include <dodo2/model/hardware/PropertyManager.hpp>


namespace dodo
{
namespace model
{
namespace hardware
{
    // helper macro to reduce code clutter inside the class
    // creates a map to store attributes and a wrapper that is suitable
    // for use in the PropertyManager
    #define CREATE_PROP_MAP( KEY_TYPE, MAPPED_TYPE, NAME )\
        std::map< KEY_TYPE, MAPPED_TYPE > internal_##NAME;\
        PropertyManager::MapType < decltype( internal_##NAME ) > NAME;


    class HardwareAbstractionBase
    {
    public:
        using MemoryHierarchyGraph = graph::TreeIDGraph;
        using ConsistsOfGraph = graph::TreeIDGraph;
        using InterconnectGraph = graph::InterconnectGraph;
        using HardwareID = ConsistsOfGraph::TreeID;
        // These graphs represent the STRUCTURE, not the attributes
    protected:
        ConsistsOfGraph cog;
        MemoryHierarchyGraph mhg;
        InterconnectGraph ig;
    private:

        // The maps store the most basic attributes.
        // To add more advanced attributes, derive from this class
        // TODO: Think about good possibilities to store the maps directly
        // inside the PropertyManager itself. Either using boost::any or
        // give ALL the types with a tuple-structure. The latter might
        // require boost::hana magic to deal with derived classes of
        // the Hardware abstraction.
        CREATE_PROP_MAP( HardwareID, std::string, nameMap )
        CREATE_PROP_MAP( InterconnectGraph::EdgeID, std::string, edgeNameMap )
        CREATE_PROP_MAP( HardwareID, property::VertexType, typeMap)

        HardwareID
        addInternal(
            std::string & childName,
            property::VertexType type,
            HardwareID childTreeID
        )
        {
            nameMap[childTreeID] = childName;
            typeMap[childTreeID] = type;

            cog.addVertex( childTreeID );
            ig.addVertex( childTreeID );
            if( type == property::VertexType::MEMORY
                || type == property::VertexType::COMPUTE
                || type == property::VertexType::CACHE
                )
            {
                mhg.addVertex( childTreeID );
            }
            return childTreeID;
        }

        bool
        isLeaf(
            const HardwareID & c
        ) const
        {
            auto outEdges = cog.getOutEdges(c);
            return outEdges.first == outEdges.second;
        }

    protected:
        PropertyManager propertyManager;

    public:

        // The 'internal_' member variables are created by the macro
        // CREATE_PROP_MAP
        HardwareAbstractionBase() :
            nameMap( internal_nameMap ),
            edgeNameMap( internal_edgeNameMap ),
            typeMap( internal_typeMap )
        {
            propertyManager.registerProperty(
                "VertexName",
                nameMap
            );
            propertyManager.registerProperty(
                "EdgeName",
                edgeNameMap
            );
            propertyManager.registerProperty(
                "VertexType",
                typeMap
            );
        }

        template<typename T>
        T
        getProperty(
            const std::string & propName,
            const utility::TreeID & id
        ) {
            return propertyManager.get(propName, id);
        }


        template<typename T>
        void
        setProperty(
            const std::string & propName,
            const HardwareID & id,
            T property
        ) {
            propertyManager.set( propName, id, property );
        }

        HardwareID
        addRoot(
            std::string childName,
            property::VertexType type
        )
        {


            assert(internal_nameMap.size() == 0u);
            return addInternal(
                childName,
                type,
                HardwareID( )
            );
        }

        HardwareID add(
            std::string childName,
            property::VertexType type,
            HardwareID & parent
        )
        {
            auto res = addInternal(
                childName,
                type,
                parent.genChildID( )
            );
            cog.addEdge(parent, res);
            return res;

        }

        template<typename T_Graph, typename T_Map>
        auto
        createIndexMap(
            T_Map & sourceMap,
            const T_Graph & graph
        )
        {
            using IndexMap = std::map<
                typename T_Graph::VertexID,
                typename T_Map::mapped_type
            >;
            IndexMap indexMap;

            for( auto v : boost::make_iterator_range( graph.getVertices() ))
            {
                utility::TreeID id = const_cast<T_Graph &>(graph)[v];
                indexMap.insert(
                    std::make_pair(
                        v,
                        sourceMap[id]
                    )
                );
            }
            return indexMap;
        }

        auto
        addInterconnect(
            HardwareID fromElement,
            HardwareID toElement,
            const std::string & name = ""
        )
        {
            InterconnectGraph::EdgeID edge = ig.addEdge(fromElement, toElement);
            propertyManager.set("EdgeName", edge, name);
            return std::vector<InterconnectGraph::EdgeID>{edge};
        }

        auto
        addInterconnectBidirectional(
            HardwareID element1,
            HardwareID element2
        )
        {
            return std::vector<InterconnectGraph::EdgeID>{
                addInterconnect(element1, element2)[0],
                addInterconnect(element2, element1)[0],
            };
        }

        auto
        getHWElementsByType(
            property::VertexType t
        )
        {
            std::vector< HardwareID > res;
            for( auto & i : internal_typeMap )
            {
                if( i.second == t )
                {
                    res.push_back( i.first );
                }
            }
            return res;
        }

        auto
        getDirectChildren(
            const HardwareID & myself
        )
        {
            std::vector< HardwareID > res;
            auto outEdges = cog.getOutEdges( myself );
            for( auto i = outEdges.first; i != outEdges.second; ++i )
            {
                res.push_back( cog[ i -> m_target ] );
            }
            return res;
        }

        auto
        getAllChildren(
            const HardwareID& myself
        )
        -> std::vector< HardwareID >
        {
            std::list< HardwareID> fringe;
            fringe.push_back(myself);
            std::vector< HardwareID > res;
            while(!fringe.empty())
            {
                auto a = fringe.front();
                fringe.pop_front();
                auto children = getDirectChildren(a);
                res.insert(res.end(), children.begin(), children.end());
                fringe.insert(fringe.end(), children.begin(), children.end());

            }

            return res;
        }


        auto
        getLeafChildren(
            const HardwareID & myself
        )
        {
            std::vector<HardwareID> res;
            auto allC = getAllChildren(myself);
            for(auto c : allC)
            {
                auto outEdges = cog.getOutEdges(c);
                if(outEdges.first == outEdges.second)
                {
                    res.push_back(c);
                }
            }
            return res;
        }


        auto
        getParent(
            HardwareID myself
        )
        {
            auto v = cog.getInEdges( myself ).first->m_source;

            return cog[v];

        }


        auto
        getReachableNeighbors(
            HardwareID myself
        )
        {
            std::list<
                std::pair<
                    HardwareID,
                    InterconnectGraph::EdgeID
                >
            > res;
            auto outEdges = ig.getOutEdges( myself );
            for( auto i = outEdges.first; i != outEdges.second; ++i )
            {
                res.push_back( std::make_pair( ig[ i -> m_target ], *i ) );
            }
            return res;
        }


        template<typename T_Predicate>
        auto filterVertices(
            const T_Predicate predicate
        ){
            std::vector< HardwareID > result;
            for(auto i : internal_nameMap)
            {
                if(predicate(i.first))
                {
                    result.push_back(i.first);
                }
            }
            return result;
        }

        auto
        getBackingMemories(
            const MemoryHierarchyGraph::TreeID & pe
        )
        {
            // Breadth first search. We assume a TREE (only 1 path to a node!)
            std::vector<MemoryHierarchyGraph::TreeID> res;
            std::list<MemoryHierarchyGraph::TreeID> fringe;
            std::set<MemoryHierarchyGraph::TreeID> visited;
            fringe.push_back(pe);
            while( ! fringe.empty() )
            {
                MemoryHierarchyGraph::TreeID current = fringe.front();
                fringe.pop_front();
                auto t = mhg.getOutEdges( current );
                for(auto i=t.first; i!=t.second; ++i)
                {
                    auto child = mhg[ ( *i ).m_target ];
                    if( std::find(res.begin(), res.end(), child) == res.end() )
                    {
                        assert(
                            propertyManager.get( "NodeType", child )
                            == property::VertexType::MEMORY
                            ||
                            propertyManager.get( "NodeType", child )
                            == property::VertexType::CACHE
                            &&
                            "Memory hierarchy seems to contain an "
                            "element that is not a memory"
                        );
                        fringe.push_back( child );
                        res.push_back( child );
                    }
                }
            }
            return res;
        }



        auto
        addToMemHierarchy(
            MemoryHierarchyGraph::TreeID backedElement,
            MemoryHierarchyGraph::TreeID backingMemory
        )
        {
            return mhg.addEdge(
                backedElement,
                backingMemory
            );
        }




    };

#undef CREATE_PROP_MAP
} /* hardware */
} /* model */
} /* dodo */

