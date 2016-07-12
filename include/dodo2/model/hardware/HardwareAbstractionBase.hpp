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

    using MemoryHierarchyGraph = graph::TreeIDGraph;
    using ConsistsOfGraph = graph::TreeIDGraph;
    using InterconnectGraph = graph::InterconnectGraph;

    class HardwareAbstractionBase
    {
        // These graphs represent the STRUCTURE, not the attributes
        ConsistsOfGraph cog;
        MemoryHierarchyGraph mhg;
        InterconnectGraph ig;

        // The maps store the most basic attributes.
        // To add more advanced attributes, derive from this class
        // TODO: Think about good possibilities to store the maps directly
        // inside the PropertyManager itself. Either using boost::any or
        // give ALL the types with a tuple-structure. The latter might
        // require boost::hana magic to deal with derived classes of
        // the Hardware abstraction.
        CREATE_PROP_MAP( ConsistsOfGraph::TreeID, std::string, nameMap )
        CREATE_PROP_MAP( InterconnectGraph::EdgeID, std::string, edgeNameMap )
        CREATE_PROP_MAP( ConsistsOfGraph::TreeID, property::VertexType, typeMap)

        ConsistsOfGraph::TreeID
        addInternal(
            std::string & childName,
            property::VertexType type,
            ConsistsOfGraph::TreeID childTreeID
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
            const ConsistsOfGraph::TreeID & c
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
            const utility::TreeID & id,
            T property
        ) {
            propertyManager.set( propName, id, property );
        }

        ConsistsOfGraph::TreeID
        addRoot(
            std::string childName,
            property::VertexType type
        )
        {


            assert(internal_nameMap.size() == 0u);
            return addInternal(
                childName,
                type,
                ConsistsOfGraph::TreeID( )
            );
        }

        ConsistsOfGraph::TreeID add(
            std::string childName,
            property::VertexType type,
            ConsistsOfGraph::TreeID & parent
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

        auto
        addInterconnect(
            ConsistsOfGraph::TreeID fromElement,
            ConsistsOfGraph::TreeID toElement,
            const std::string & name = ""
        )
        {
            InterconnectGraph::EdgeID edge = ig.addEdge(fromElement, toElement);
            propertyManager.set("EdgeName", edge, name);
            return std::vector<InterconnectGraph::EdgeID>{edge};
        }

        auto
        addInterconnectBidirectional(
            ConsistsOfGraph::TreeID element1,
            ConsistsOfGraph::TreeID element2
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
            std::vector< ConsistsOfGraph::TreeID > res;
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
            const graph::TreeIDGraph::TreeID& myself
        )
        {
            std::vector<graph::TreeIDGraph::TreeID> res;
            auto outEdges = cog.getOutEdges( myself );
            for( auto i = outEdges.first; i != outEdges.second; ++i )
            {
                res.push_back( cog[ i -> m_target ] );
            }
            return res;
        }

        auto
        getAllChildren(
            const ConsistsOfGraph::TreeID& myself
        )
        {
            std::list< ConsistsOfGraph::TreeID> fringe;
            fringe.push_back(myself);
            std::vector< ConsistsOfGraph::TreeID > res;
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
            const graph::TreeIDGraph::TreeID& myself
        )
        {
            std::vector<ConsistsOfGraph::TreeID> res;
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
            ConsistsOfGraph::TreeID myself
        )
        {
            auto v = cog.getInEdges( myself ).first->m_source;

            return cog[v];

        }


        auto
        getReachableNeighbors(
            ConsistsOfGraph::TreeID myself
        )
        {
            std::list<
                std::pair<
                    ConsistsOfGraph::TreeID,
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
            std::vector< ConsistsOfGraph::TreeID > result;
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

