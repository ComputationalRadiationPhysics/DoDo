#pragma once


#include "../graph/TreeIDGraph.hpp"
#include "TransferGraph.hpp"
#include "NodeType.hpp"
#include "MemoryCapacity.hpp"
#include "Bandwidth.hpp"


namespace dodo
{
    namespace hardware
    {
        struct HardwareAbstraction
        {
            // These graphs represent the STRUCTURE, not the attributes
            graph::TreeIDGraph cog;
            graph::TreeIDGraph mhg;
            TransferGraph ig;

            // These maps represent attributes based on one of the structures
            // (for Vertices, the index is always a TreeID that is used internally in the graphs)
            // for Edges, the IDs are taken from the graph directly (might change)
            std::map<
                graph::TreeIDGraph::TreeID,
                std::string
            > nameMap;

            std::map<
                TransferGraph::EdgeID,
                std::string
            > cableNameMap;

            std::map<
                graph::TreeIDGraph::TreeID,
                NodeType
            > typeMap;

            std::map<
                graph::TreeIDGraph::TreeID,
                MemoryCapacity
            > capacityMap;

            std::map<
                TransferGraph::EdgeID,
                Bandwidth
            > bandwidthMap;

            std::map<
                graph::TreeIDGraph::TreeID,
                size_t
            > gigaFLOPSMap;


            template<typename T_Map>
            void
            writeMap(std::string path, const T_Map& map)
            {
                std::ofstream ofs;
                ofs.open(path);
                for(auto m : map)
                {
                    ofs << m.first << " " << m.second << std::endl;
                }
                ofs.close();
            }

            void
            writeAllMaps(std::string path_base)
            {
                writeMap(path_base + "nameMap.txt", nameMap);
                writeMap(path_base + "cableNameMap.txt", cableNameMap);
                writeMap(path_base + "typeMap.txt", typeMap);
                writeMap(path_base + "capacityMap.txt", capacityMap);
                writeMap(path_base + "bandwidthMap.txt", bandwidthMap);
                writeMap(path_base + "gigaFLOPSMap.txt", gigaFLOPSMap);
            }

            template<typename T_Graph, typename T_Map>
            auto
            createIndexMap(T_Map & sourceMap, const T_Graph & graph)
            {
                using IndexMap = std::map<typename T_Graph::VertexID, typename T_Map::mapped_type>;
                IndexMap indexMap;

                for(decltype(auto) v : boost::make_iterator_range(boost::vertices(*graph.graph)))
                {
                    utility::TreeID id = const_cast<T_Graph&>(graph)[v];
                    indexMap.insert(std::make_pair(v, sourceMap[id]));
                }
                return indexMap;
            }


            template<typename T_Graph>
            void
            writeTreeIDGraph
                (
                    const T_Graph & graph,
                    std::string path
                )
            {
                std::ofstream ofs;
                ofs.open(path);
                boost::dynamic_properties dp;
                dp.property(
                    "TreeID",
                    get(
                        &std::decay< T_Graph >::type::TreeID::id,
                        *( graph.graph )
                    )
                );
                auto indexMap = createIndexMap(typeMap, graph);
                boost::associative_property_map<decltype(indexMap)>associativeMap(indexMap);
                dp.property( "NodeType", associativeMap );

                auto indexMap2 = createIndexMap(nameMap, graph);
                boost::associative_property_map<decltype(indexMap2)>associativeMap2(indexMap2);
                dp.property( "Name", associativeMap2 );


                write_graphml(ofs, *(graph.graph), dp);
                ofs.close();
            }

            void
            writeAllTreeIDGraphs(
                std::string path_base
            )
            {
                writeTreeIDGraph(
                    cog,
                    path_base + "consists_of_graph.graphml"
                );

                writeTreeIDGraph(
                    mhg,
                    path_base + "memory_hierarchy_graph.graphml"
                );

                writeTreeIDGraph(
                    ig,
                    path_base + "interconnect_graph.graphml"
                );
            }

            graph::TreeIDGraph::TreeID addNode(
                std::string childName,
                NodeType type,
                graph::TreeIDGraph::TreeID & parent
            )
            {
                auto res = addNodeInternal(
                    childName,
                    type,
                    parent.genChildID()
                );
                cog.addEdge(parent, res);
                return res;

            }



            graph::TreeIDGraph::TreeID addRoot(
                std::string childName,
                NodeType type
            )
            {
                assert(nameMap.size() == 0u);
                return addNodeInternal(
                    childName,
                    type,
                    graph::TreeIDGraph::TreeID()
                );
            }

            template<typename T_Predicate>
            auto filterVertices(
                const T_Predicate predicate
            ){
                std::vector<graph::TreeIDGraph::TreeID> result;
                for(auto i : nameMap)
                {
                    if(predicate(i.first))
                    {
                        result.push_back(i.first);
                    }
                }
                return result;
            }


            graph::TreeIDGraph::TreeID
            addNodeInternal
            (
                std::string & childName,
                NodeType type,
                graph::TreeIDGraph::TreeID childTreeID
            )
            {
                nameMap[childTreeID] = childName;
                typeMap.insert(std::make_pair(childTreeID, type));

                cog.addVertex( childTreeID );
                ig.addVertex( childTreeID );
                if( type == NodeType::MEMORY
                    || type == NodeType::COMPUTE
                    || type == NodeType::CACHE
                )
                {
                    mhg.addVertex( childTreeID );
                }
                return childTreeID;
            }


            auto
            addInterconnect(
                graph::TreeIDGraph::TreeID fromElement,
                graph::TreeIDGraph::TreeID toElement,
                std::string name = ""
            )
            {
                auto edge = ig.addEdge(fromElement, toElement);
                cableNameMap.insert(std::make_pair(edge, name));
                return std::vector<TransferGraph::EdgeID>{edge};
            }

            auto
            addInterconnectBidirectional(
                graph::TreeIDGraph::TreeID element1,
                graph::TreeIDGraph::TreeID element2,
                std::string name = ""
            )
            {
                return std::vector<TransferGraph::EdgeID>{
                    addInterconnect(element1, element2, name)[0],
                    addInterconnect(element2, element1, name)[0],
                };
            }


            auto
            getHWElementsByType(
                NodeType t
            )
            {
                std::vector< graph::TreeIDGraph::TreeID > res;
                for( auto & i : typeMap )
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
                const graph::TreeIDGraph::TreeID& myself
            )
            {
                std::list< graph::TreeIDGraph::TreeID> fringe;
                fringe.push_back(myself);
                std::vector< graph::TreeIDGraph::TreeID > res;
                while(!fringe.empty())
                {
                    auto a = fringe.front();
                    fringe.pop_front();
                    auto children = getDirectChildren(a);
//                    for(auto c : children)
//                    {
//                        res.push_back( c );
//                    }
                    res.insert(res.end(), children.begin(), children.end());
                    fringe.insert(fringe.end(), children.begin(), children.end());

                }

                return res;
            }


            bool
            isLeaf(
                const graph::TreeIDGraph::TreeID & c
            ) const
            {
                auto outEdges = cog.getOutEdges(c);
                return outEdges.first == outEdges.second;
            }


            auto
            getLeafChildren(
                const graph::TreeIDGraph::TreeID& myself
            )
            {
                std::vector<graph::TreeIDGraph::TreeID> res;
                auto allC = getAllChildren(myself);
//                std::copy_if(
//                    allC.begin( ),
//                    allC.end( ),
//                    res.begin( ),
//                    std::bind(
//                        &HardwareAbstraction::isLeaf,
//                        this,
//                        std::placeholders::_1
//                    )
//                );

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
                graph::TreeIDGraph::TreeID myself
            )
            {
                auto v = cog.getInEdges( myself ).first->m_source;

                return cog[v];

            }


            auto
            getReachableNeighbors(
                graph::TreeIDGraph::TreeID myself
            )
            {
                std::list<
                    std::pair<
                        graph::TreeIDGraph::TreeID,
                        TransferGraph::SBGL::EdgeID
                    >
                > res;
                auto outEdges = ig.getOutEdges( myself );
                for( auto i = outEdges.first; i != outEdges.second; ++i )
                {
                    res.push_back( std::make_pair( ig[ i -> m_target ], *i ) );
                }
                return res;
            }


            auto
            getBackingMemories(
                graph::TreeIDGraph::TreeID pe
            )
            {
                // Breadth first search. We assume a TREE (only 1 path to a node!)
                std::vector<graph::TreeIDGraph::TreeID> res;
                std::list<graph::TreeIDGraph::TreeID> fringe;
                std::set<graph::TreeIDGraph::TreeID> visited;
                fringe.push_back(pe);
                while( ! fringe.empty() )
                {
                    graph::TreeIDGraph::TreeID current = fringe.front();
                    fringe.pop_front();
                    auto t = mhg.getOutEdges( current );
                    for(auto i=t.first; i!=t.second; ++i)
                    {
                        auto child = mhg[ ( *i ).m_target ];
                        if( std::find(res.begin(), res.end(), child) == res.end() )
                        {
                            assert( (typeMap.at( child ) == NodeType::MEMORY ||
                                    typeMap.at( child ) == NodeType::CACHE) &&
                                    "There is a non-memory element "
                                    "in the Memory Hierarchy Graph!"
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
                graph::TreeIDGraph::TreeID backedElement,
                graph::TreeIDGraph::TreeID backingMemory
            )
            {
                return mhg.addEdge(
                    backedElement,
                    backingMemory
                );
            }


            void
            setCapacity(
                graph::TreeIDGraph::TreeID memory,
                std::size_t capacityInKByte
            ){
                assert( typeMap.at( memory ) == NodeType::MEMORY ||
                        typeMap.at( memory ) == NodeType::CACHE &&
                        "Trying to set ");
                MemoryCapacity m { capacityInKByte };
                capacityMap.insert( std::make_pair(memory, m));
                //capacityMap.at( memory ) = m;
            }





        };

    }
}

