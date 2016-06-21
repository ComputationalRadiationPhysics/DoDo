#pragma once

#include <boost/fusion/include/define_struct.hpp>

#include "dodo/graph/SimpleBGL.hpp"
#include "dodo/utility/tree_id.hpp"

namespace dodo
{
    namespace hardware
    {

        class TransferGraph :
            public graph::SimpleBGL< utility::TreeID, size_t, boost::multisetS >
        {
        public:
            using TreeID = utility::TreeID;
            using SBGL = graph::SimpleBGL< TreeID, size_t, boost::multisetS >;
            using VertexID = SBGL::VertexID;
            using EdgeID = SBGL::EdgeID;

        private:
            std::map<
                TreeID,
                VertexID
            > idmap;


        public:

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
                    idmap[srcVertex],
                    idmap[targetVertex]
                );
            }

            auto
            getAdjacentVertices( const TreeID & v )
            {
                return SBGL::getAdjacentVertices(idmap[v]);
            }

            auto
            getOutEdges( const TreeID & v )
            {
                return SBGL::getOutEdges(idmap[v]);
            }


            auto
            operator[]( const VertexID & v)
            {
                return (*graph)[v];
            }

            auto
            operator[]( const TreeID & v)
            {
                return (*graph)[idmap[v]];
            }



        };


    }
}
