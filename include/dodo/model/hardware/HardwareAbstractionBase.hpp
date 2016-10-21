#pragma once

#include <list>
#include <cassert>

#include <boost/range/iterator_range.hpp>

#include <dodo/graph/TreeIDGraph.hpp>
#include <dodo/graph/InterconnectGraph.hpp>
#include <dodo/model/hardware/property/VertexType.hpp>
#include <dodo/utility/tuple_index.hpp>
#include <dodo/utility/PropertyManager.hpp>


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
        utility::PropertyManager::MapType < decltype( internal_##NAME ) > NAME;


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

                // The maps store the most basic attributes.
                // To add more advanced attributes, derive from this class
                // TODO: Think about good possibilities to store the maps directly
                // inside the PropertyManager itself. Either using boost::any or
                // give ALL the types with a tuple-structure. The latter might
                // require boost::hana magic to deal with derived classes of
                // the Hardware abstraction.
                CREATE_PROP_MAP( HardwareID,
                    std::string,
                    nameMap )
                CREATE_PROP_MAP( InterconnectGraph::EdgeID,
                    std::string,
                    edgeNameMap )
                CREATE_PROP_MAP( HardwareID,
                    property::VertexType,
                    typeMap )

            private:
                HardwareID addInternal(
                    std::string & childName,
                    property::VertexType type,
                    HardwareID childTreeID
                )
                {
                    nameMap[childTreeID] = childName;
                    typeMap[childTreeID] = type;

                    cog.addVertex( childTreeID );
                    ig.addVertex( childTreeID );
                    if( type == property::VertexType::MEMORY ||
                        type == property::VertexType::COMPUTE ||
                        type == property::VertexType::CACHE )
                    {
                        mhg.addVertex( childTreeID );
                    }
                    return childTreeID;
                }


                bool isLeaf(
                    const HardwareID & c
                ) const
                {
                    auto outEdges = cog.getOutEdges( c );
                    return outEdges.first == outEdges.second;
                }


            protected:
                utility::PropertyManager propertyManager;

                std::size_t
                countPropertiesBase()
                {
                    return internal_nameMap.size() +
                           internal_edgeNameMap.size() +
                           internal_typeMap.size();
                }

            public:

                // The 'internal_' member variables are created by the macro
                // CREATE_PROP_MAP
                HardwareAbstractionBase( ) :
                    cog(),
                    mhg(),
                    ig(),
                    internal_nameMap(),
                    nameMap( internal_nameMap ),
                    internal_edgeNameMap(),
                    edgeNameMap( internal_edgeNameMap ),
                    internal_typeMap(),
                    typeMap( internal_typeMap ),
                    propertyManager()
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

                virtual ~HardwareAbstractionBase() = default;

                auto
                isVertexType(
                    const HardwareID & id,
                    property::VertexType t
                ) const
                -> bool
                {
                    return internal_typeMap.at(id) == t;
                }

                template<
                    typename T,
                    typename I
                >
                T getProperty(
                    const std::string & propName,
                    const I & id
                )
                {
                    return propertyManager.get< T >(
                        propName,
                        id
                    );
                }


                template<
                    typename T,
                    typename I
                >
                void setProperty(
                    const std::string & propName,
                    const I & id,
                    T property
                )
                {
                    propertyManager.set(
                        propName,
                        id,
                        property
                    );
                }


                template<
                    typename T
                >
                void setProperty(
                    const std::string & propName,
                    const std::vector<InterconnectGraph::EdgeID> & ids,
                    T property
                )
                {
                    for( InterconnectGraph::EdgeID id : ids)
                    {
                        propertyManager.set(
                            propName,
                            id,
                            property
                        );
                    }
                }

                HardwareID addRoot(
                    std::string childName,
                    property::VertexType type
                )
                {


                    assert( internal_nameMap.size( ) == 0u );
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
                    cog.addEdge(
                        parent,
                        res
                    );
                    return res;

                }


                template<
                    typename T_Graph,
                    typename T_Map
                >
                auto createIndexMap(
                    T_Map & sourceMap,
                    const T_Graph & graph
                )
                {

                    using IndexMap = std::map<
                        typename T_Graph::VertexID,
                        typename T_Map::mapped_type
                    >;
                    auto indexMap = std::make_shared< IndexMap >( );

                    for( auto v : boost::make_iterator_range( graph.getVertices( ) ) )
                    {
                        utility::TreeID id = const_cast<T_Graph &>(graph)[v];
                        indexMap->insert(
                            std::make_pair(
                                v,
                                sourceMap[id]
                            )
                        );
                    }
                    return indexMap;
                }


                template<
                    typename T_Graph,
                    typename T_Map
                >
                auto createAssociativeIndexMap(
                    T_Map & sourceMap,
                    const T_Graph & graph,
                    std::list< std::shared_ptr< void > > & freeList
                )
                {

                    auto indexMap = createIndexMap(
                        sourceMap,
                        graph
                    );
                    freeList.push_back( indexMap );
                    boost::associative_property_map< typename std::decay< decltype( *indexMap ) >::type >
                        associativeMap( *indexMap );
                    return associativeMap;
                }


                auto addInterconnect(
                    HardwareID fromElement,
                    HardwareID toElement,
                    const std::string & name = ""
                )
                {
                    InterconnectGraph::EdgeID
                        edge =
                        ig.addEdge(
                            fromElement,
                            toElement
                        );
                    propertyManager.set(
                        "EdgeName",
                        edge,
                        name
                    );
                    return std::vector< InterconnectGraph::EdgeID >{ edge };
                }


                auto addInterconnectBidirectional(
                    HardwareID element1,
                    HardwareID element2,
                    const std::string & name = ""
                )
                {
                    return std::vector< InterconnectGraph::EdgeID >{
                        addInterconnect(
                            element1,
                            element2,
                            name
                        )[0],
                        addInterconnect(
                            element2,
                            element1,
                            name
                        )[0]
                    };
                }


                auto
                getHWElementsByType(
                    property::VertexType t
                )
                -> std::vector< HardwareID >
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
                getHWElementsByName(
                    const std::string t
                )
                -> std::vector< HardwareID >
                {
                    std::vector< HardwareID > res;
                    for( auto & i : internal_nameMap )
                    {
                        if( i.second == t )
                        {
                            res.push_back( i.first );
                        }
                    }
                    return res;
                }


                auto getDirectChildren(
                    const HardwareID & myself
                )
                -> std::vector< HardwareID >
                {
                    std::vector< HardwareID > res;
                    auto outEdges = cog.getOutEdges( myself );
                    for( auto i = outEdges.first; i != outEdges.second; ++i )
                    {
                        res.push_back( cog[i->m_target] );
                    }
                    return res;
                }


                auto getAllChildren(
                    const HardwareID & myself
                ) -> std::vector< HardwareID >
                {
                    std::list< HardwareID > fringe;
                    fringe.push_back( myself );
                    std::vector< HardwareID > res;
                    while( !fringe.empty( ) )
                    {
                        auto a = fringe.front( );
                        fringe.pop_front( );
                        auto children = getDirectChildren( a );
                        res.insert(
                            res.end( ),
                            children.begin( ),
                            children.end( )
                        );
                        fringe.insert(
                            fringe.end( ),
                            children.begin( ),
                            children.end( )
                        );

                    }

                    return res;
                }


                auto getAllChildrenWithType(
                    const HardwareID & myself,
                    const property::VertexType t
                ) -> std::vector< HardwareID >
                {
                    std::list< HardwareID > fringe;
                    fringe.push_back( myself );
                    std::vector< HardwareID > res;
                    while( !fringe.empty( ) )
                    {
                        auto a = fringe.front( );
                        fringe.pop_front( );
                        auto children = getDirectChildren( a );
                        for(auto & c : children)
                        {
                            if(typeMap[c] == t)
                            {
                                res.push_back(c);
                            }
                        }
//                        res.insert(
//                            res.end( ),
//                            children.begin( ),
//                            children.end( )
//                        );
                        fringe.insert(
                            fringe.end( ),
                            children.begin( ),
                            children.end( )
                        );

                    }

                    return res;
                }

                auto
                getLeafChildren(
                    const HardwareID & myself
                )
                -> std::vector< HardwareID >
                {
                    std::vector< HardwareID > res;
                    auto allC = getAllChildren( myself );
                    for( auto c : allC )
                    {
                        auto outEdges = cog.getOutEdges( c );
                        if( outEdges.first == outEdges.second )
                        {
                            res.push_back( c );
                        }
                    }
                    return res;
                }


                auto
                getParent(
                    const HardwareID & myself
                )
                -> HardwareID
                {
                    auto inEdgePair = cog.getInEdges( myself );
                    if( std::distance(inEdgePair.first, inEdgePair.second) == 0)
                    {
                        throw std::runtime_error("The Node " + boost::lexical_cast(myself) + " does not have any parents!");
                    }
                    auto v = inEdgePair.first->m_source;
                    return cog[v];
                }

                auto
                isIndirectParent(
                    const HardwareID & possibleParent,
                    const HardwareID & possibleChild
                )
                -> bool
                {
                    return possibleParent.hasIndirectChild(possibleChild);
                }


                auto getReachableNeighbors(
                    HardwareID myself
                )
                -> std::list< std::pair<
                    HardwareID,
                    InterconnectGraph::EdgeID
                > >
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
                        res.push_back(
                            std::make_pair(
                                ig[i->m_target],
                                *i
                            )
                        );
                    }
                    return res;
                }


                template< typename T_Predicate >
                auto filterVertices(
                    const T_Predicate predicate
                )
                -> std::vector< HardwareID >
                {
                    std::vector< HardwareID > result;
                    for( auto i : internal_nameMap )
                    {
                        if( predicate( i.first ) )
                        {
                            result.push_back( i.first );
                        }
                    }
                    return result;
                }


                auto getBackingMemories(
                    const MemoryHierarchyGraph::TreeID & pe
                ) -> std::vector< MemoryHierarchyGraph::TreeID >
                {
                    // Breadth first search. We assume a TREE (only 1 path to a node!)
                    std::vector< MemoryHierarchyGraph::TreeID > res;
                    std::list< MemoryHierarchyGraph::TreeID > fringe;
                    std::set< MemoryHierarchyGraph::TreeID > visited;
                    fringe.push_back( pe );
                    while( !fringe.empty( ) )
                    {
                        MemoryHierarchyGraph::TreeID current = fringe.front( );
                        fringe.pop_front( );
                        auto t = mhg.getOutEdges( current );
                        for( auto i = t.first; i != t.second; ++i )
                        {
                            MemoryHierarchyGraph::TreeID
                                child = mhg[( *i ).m_target];
                            if( std::find(
                                res.begin( ),
                                res.end( ),
                                child
                            ) == res.end( ) )
                            {
                                assert( ( propertyManager.get< property::VertexType >(
                                    "VertexType",
                                    child
                                ) == property::VertexType::MEMORY ||
                                          propertyManager.get< property::VertexType >(
                                              "VertexType",
                                              child
                                          ) == property::VertexType::CACHE ) &&
                                        "Memory hierarchy seems to contain an "
                                            "element that is not a memory" );
                                fringe.push_back( child );
                                res.push_back( child );
                            }
                        }
                    }
                    return res;
                }


                auto addToMemHierarchy(
                    MemoryHierarchyGraph::TreeID backedElement,
                    MemoryHierarchyGraph::TreeID backingMemory
                )
                {
                    return mhg.addEdge(
                        backedElement,
                        backingMemory
                    );
                }


                auto getAllInterconnects( )
                {
                    return ig.getEdges( );
                }

                auto
                getInterconnectsByName(
                    const std::string & t
                )
                -> std::vector< InterconnectGraph::EdgeID >
                {
                    std::vector< InterconnectGraph::EdgeID > res;
                    for( auto & i : internal_edgeNameMap )
                    {
                        if( i.second == t )
                        {
                            res.push_back( i.first );
                        }
                    }
                    return res;
                }


            };

#undef CREATE_PROP_MAP
        } /* hardware */
    } /* model */
} /* dodo */

