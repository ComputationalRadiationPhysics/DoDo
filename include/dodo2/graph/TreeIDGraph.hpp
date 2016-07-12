#pragma once

#include "dodo2/graph/SimpleBGL.hpp"
#include "dodo2/utility/tree_id.hpp"

namespace dodo
{
namespace graph
{

    class TreeIDGraph :
        public graph::SimpleBGL< utility::TreeID >
    {
    public:
        using TreeID = utility::TreeID;
        using SBGL = graph::SimpleBGL< TreeID >;
        using VertexID = SBGL::VertexID;

        using IdMap = std::map<
            TreeID,
            VertexID
        >;
        IdMap idmap;


        void
        addVertexHook(
            const TreeID & tid,
            const VertexID & v
        ) override
        {
            idmap[tid] = v;
        }


        SBGL::EdgeID
        addEdge(
            const VertexID & srcVertex,
            const VertexID & targetVertex
        )
        {
            return SBGL::addEdge(
                srcVertex,
                targetVertex
            );
        }

        SBGL::EdgeID
        addEdge(
            const TreeID & srcVertex,
            const TreeID & targetVertex
        )
        {
            return SBGL::addEdge(
                idmap.at( srcVertex ),
                idmap.at( targetVertex )
            );
        }


        auto
        getAdjacentVertices( const TreeID & v )
        {
            return SBGL::getAdjacentVertices( idmap.at( v ) );
        }

        auto
        getOutEdges( const TreeID & v ) const
        {
            return SBGL::getOutEdges( idmap.at( v ) );
        }

        auto
        getInEdges( const TreeID & v )
        {
            return SBGL::getInEdges( idmap.at( v ) );
        }


        auto
        operator[]( const VertexID & v)
        {
            return ( *graph )[v];
        }

        auto
        operator[]( const TreeID & v)
        {
            return ( *graph )[idmap.at( v )];
        }

    };


} /* graph */
} /* dodo */
