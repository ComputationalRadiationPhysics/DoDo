#pragma once

#include <boost/fusion/include/define_struct.hpp>

#include "dodo/graph/SimpleBGL.hpp"
#include "dodo/utility/tree_id.hpp"

namespace dodo
{
    namespace hardware
    {

        class ConsistsOfGraph :
            public graph::SimpleBGL< utility::TreeID >
        {
        public:
            using TreeID = utility::TreeID;
            using SBGL = graph::SimpleBGL< TreeID >;
            using VertexID = SBGL::VertexID;

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
//                return boost::add_edge(
//                    idmap[srcVertex],
//                    idmap[targetVertex],
//                    ( *graph )
//                ).first;
                return SBGL::addEdge(
                    idmap[srcVertex],
                    idmap[targetVertex]
                );
            }

        };


    }
}
