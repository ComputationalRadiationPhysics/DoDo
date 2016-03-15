#pragma once


#include "HardwareGraphVertex.hpp"
#include "InterconnectGraph.hpp"
#include "dodo/utility/tree_id.hpp"
#include "dodo/utility/convert_tuple.hpp"


namespace dodo
{
    namespace hardware
    {

        template<typename T_Vertex>
        class HardwareGraph
        {
            using ConsistsOfPropertes = typename T_Vertex::Properties;
            using InterconnectProperties = typename T_Vertex::InterconnectProperties;
            using VertexBase_t = typename T_Vertex::ConsistsOfStructure::value_type;

        public:
            using Vertex = T_Vertex;
            using Interconnect_t = InterconnectGraph<InterconnectProperties>;

        private:
            std::shared_ptr<Interconnect_t> interconnectGraph;
            Vertex rootVertex;

        public:
            HardwareGraph() :
                interconnectGraph{std::make_shared<Interconnect_t>()},
                rootVertex{utility::TreeID(), interconnectGraph}
            {}


            auto extractData()
            {
                return extractData(
                    [](auto &)
                    {return true;}
                );
            }


            template<
                typename T_NewVertexProperties = ConsistsOfPropertes,
                typename T_NewEdgeProperties = InterconnectProperties,
                typename T_Predicate
            >
            auto
            extractData(const T_Predicate predicate)
            {
                // dout::Dout& dout{ dout::Dout::getInstance() };
                using InterconnectID = typename Vertex::InterconnectID;
                using IsoMap = std::map<InterconnectID, InterconnectID>;
                using VertexList = std::list<VertexBase_t>;
                using NewInterconnect_t = InterconnectGraph<
                    T_NewEdgeProperties,
                    T_NewVertexProperties
                >;

                VertexList openVertices;
                VertexList to_delete;
                VertexList to_keep;
                IsoMap isoMap;
                auto reducedIGraph = copyAndTransform<NewInterconnect_t>(interconnectGraph, isoMap);
                openVertices.push_back(rootVertex);

                while (!openVertices.empty())
                {
                    VertexBase_t current{openVertices.front()};
                    openVertices.pop_front();
                    openVertices.insert(
                        openVertices.end(),
                        current.children.begin(),
                        current.children.end());

                    if (predicate(current))
                    {
                        const auto pos(current.id);
                        reducedIGraph->mapping[pos] = isoMap[interconnectGraph->mapping[pos]];
                        reducedIGraph->stableMapping[pos] = interconnectGraph->stableMapping[pos];
                        to_keep.push_back(current);
                    } else
                    {
                        to_delete.push_back(current);
                    }
                }

                typename NewInterconnect_t::EdgeHistoryMap edgeHistoryMap = initEdgeHistory(
                    reducedIGraph
                );
                for (const VertexBase_t &v : to_delete)
                {
                    const auto iid(isoMap[interconnectGraph->mapping[v.id]]);
                    // reducedIGraph->mergeStarTopology(iid, edgeHistoryMap);
                    reducedIGraph->mergeStarTopologyUndirected(iid, edgeHistoryMap);
                }

                for (const VertexBase_t &v : to_keep)
                {
                    const auto iid(reducedIGraph->mapping[v.id]);
                    utility::convertTuple(
                        v.properties,
                        reducedIGraph->getVertexProperty(iid).second
                    );
                }

                return reducedIGraph;
            }

        };


    } /* graph */
} /* dodo */
