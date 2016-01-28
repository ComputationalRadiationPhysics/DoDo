#pragma once

#include "HardwareGraphVertex.hpp"
#include "InterconnectGraph.hpp"
#include "../utility/tree_id.hpp"

namespace dodo
{
namespace graph
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
      interconnectGraph{ std::make_shared<Interconnect_t>() },
      rootVertex{utility::TreeID(), interconnectGraph}
    {}

    auto extractData() { return extractData([](auto &i){return true;}); }

    template<typename T>
    std::shared_ptr<Interconnect_t>
    extractData(const T predicate)
    {
        using InterconnectID = typename Vertex::InterconnectID;
        using IsoMap = std::map<InterconnectID, InterconnectID>;
        using VertexList = std::list<VertexBase_t>;

        VertexList openVertices;
        VertexList to_delete;
        IsoMap isoMap;
        boost::associative_property_map<IsoMap> mapWrapper{isoMap};
        auto reducedIGraph( std::make_shared<Interconnect_t>() );

        boost::copy_graph(*(interconnectGraph->graph), *(reducedIGraph->graph), boost::orig_to_copy(mapWrapper));
        openVertices.push_back(rootVertex);

        while( ! openVertices.empty() )
        {
            VertexBase_t current{ openVertices.front() };
            openVertices.pop_front();
            openVertices.insert(openVertices.end(), current.children.begin(), current.children.end());

            if( predicate(current) )
            {
                const auto pos( current.id );
                reducedIGraph->mapping[pos] = isoMap[interconnectGraph->mapping[pos]];
                reducedIGraph->stableMapping[pos] = interconnectGraph->stableMapping[pos];
            } else
            {
                to_delete.push_back(current);
            }
        }

        dout::Dout& dout{ dout::Dout::getInstance() };
        dout(dout::Flags::DEBUG) << "Processed consistsOf graph. Results for Interconnect graph:" << std::endl;
        dout(dout::Flags::DEBUG) << "Vertices:  " << std::distance(reducedIGraph->getVertices().first, reducedIGraph->getVertices().second) << std::endl;
        dout(dout::Flags::DEBUG) << "to_delete: " << to_delete.size() << std::endl;
        dout(dout::Flags::DEBUG) << "Edges:     " << std::distance(reducedIGraph->getEdges().first, reducedIGraph->getEdges().second) << std::endl;

        typename Interconnect_t::EdgeHistoryMap edgeHistoryMap = initEdgeHistory(reducedIGraph);


        for(const VertexBase_t& v : to_delete)
        {
            const auto iid( isoMap[interconnectGraph->mapping[v.id]] );
            reducedIGraph->mergeStarTopology(iid, edgeHistoryMap);
        }
        dout(dout::Flags::DEBUG) << "Interconnect graph was reduced:" << std::endl;
        dout(dout::Flags::DEBUG) << "Vertices:  " << std::distance(reducedIGraph->getVertices().first, reducedIGraph->getVertices().second) << std::endl;
        auto allReducedEdges = reducedIGraph->getEdges();
        dout(dout::Flags::DEBUG) << "Edges:     " << std::distance(allReducedEdges.first, allReducedEdges.second) << std::endl;

        // for(auto e(allReducedEdges.first) ; e!=allReducedEdges.second ; ++e)
        // {
        //     typename Interconnect_t::EdgeProperties p = reducedIGraph->getEdgeProperty(*e).second;
        //     typename dodo::physical::attributes::Bandwidth b = std::get<1>(p);
        //     dout(dout::Flags::DEBUG) << "    " << *e << ": " << b.value << std::endl;
        // }


        return reducedIGraph;
    }

};


} /* graph */
} /* dodo */
