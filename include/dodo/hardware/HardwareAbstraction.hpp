#pragma once


#include "../graph/TreeIDGraph.hpp"
#include "TransferGraph.hpp"
#include "NodeType.hpp"
#include "MemoryCapacity.hpp"


namespace dodo
{
    namespace hardware
    {
        class HardwareAbstraction
        {
            // These graphs represent the STRUCTURE, not the attributes
            TreeIDGraph cog;
            TreeIDGraph mhg;
            TransferGraph ig;

            // These maps represent attributes based on one of the structures
            // (for Vertices, the index is always a TreeID that is used internally in the graphs)
            // for Edges, the IDs are taken from the graph directly (might change)
            std::map<
                TreeIDGraph::TreeID,
                std::string
            > nameMap;

            std::map<
                TreeIDGraph::TreeID,
                NodeType
            > typeMap;

            std::map<
                TreeIDGraph::TreeID,
                MemoryCapacity
            > capacityMap;


            TreeIDGraph::TreeID addChild(
                TreeIDGraph::TreeID & parent,
                std::string childName,
                NodeType type
            )
            {
                TreeIDGraph::TreeID childTreeID = parent.genChildID( );
                nameMap[childTreeID] = childName;
                typeMap[childTreeID] = type;

                cog.addVertex( childTreeID );
                ig.addVertex( childTreeID );
                if( type == NodeType::MEMORY || type == NodeType::COMPUTE )
                {
                    mhg.addVertex( childTreeID );
                    if( type == NodeType::MEMORY )
                    {
                        capacityMap.insert( std::make_pair(childTreeID, MemoryCapacity(0u)));
                    }
                }
                return childTreeID;
            }

            auto
            addInterconnect(
                TreeIDGraph::TreeID fromElement,
                TreeIDGraph::TreeID toElement
            )
            {
                auto edge = ig.addEdge(fromElement, toElement);
                return edge;
            }

            auto
            getHWElementsByType(
                NodeType t
            )
            {
                std::vector< TreeIDGraph::TreeID > res;
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
            getReachableNeighbors(
                TreeIDGraph::TreeID myself
            )
            {
                std::list<
                    std::pair<
                        TreeIDGraph::TreeID,
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
                TreeIDGraph::TreeID pe
            )
            {
                // Breadth first search. We assume a TREE (only 1 path to a node!)
                std::vector<TreeIDGraph::TreeID> res;
                std::list<TreeIDGraph::TreeID> fringe;
                fringe.push_back(pe);
                while( ! fringe.empty() )
                {
                    TreeIDGraph::TreeID current = fringe.front();
                    fringe.pop_front();
                    auto t = mhg.getOutEdges( current );
                    for(auto i=t.first; i!=t.second; ++i)
                    {
                        auto child = mhg[(*i).m_target];
                        assert(typeMap.at(child) == NodeType::MEMORY);
                        fringe.push_back(child);
                        res.push_back(child);
                    }
                }
                return res;
            }


            auto
            addToMemHierarchy(
                TreeIDGraph::TreeID backedElement,
                TreeIDGraph::TreeID backingMemory
            )
            {
                return mhg.addEdge(
                    backedElement,
                    backingMemory
                );
            }

            void
            setCapacity(
                TreeIDGraph::TreeID memory,
                std::size_t capacityInKByte
            ){
                MemoryCapacity m { capacityInKByte };
                capacityMap.at( memory ) = m;
            }





        };

    }
}

