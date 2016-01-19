#pragma once

#include <memory>
#include <tuple>
#include <utility>

#include <boost/hana.hpp>

#include <dout/dout.hpp>

#include <dodo/graph/Property.hpp>
#include <dodo/graph/AttributeMapStore.hpp>
#include <dodo/graph/BGL.hpp>
#include <dodo/graph/HWNode.hpp>
#include <dodo/graph/InterconnectEdge.hpp>


namespace dodo
{
namespace graph
{

namespace hana = boost::hana;


template<typename T_InterconnectProperties>
class InterconnectGraph :
    public BGL<SimpleProperty, T_InterconnectProperties>
{
public:
    using Properties = T_InterconnectProperties;
    using Graph =  BGL<SimpleProperty, T_InterconnectProperties>;
    using VertexID = typename Graph::VertexID;
    using EdgeID = typename Graph::EdgeID;

    using StableVertexID = typename Graph::VertexPropertyBundle::first_type;

    using Mapping = std::map<utility::TreeID, VertexID, utility::TreeIDLess>;
    using StableMapping = std::map<utility::TreeID, StableVertexID, utility::TreeIDLess>;

    Mapping mapping;
    StableMapping stableMapping;

    VertexID add(const utility::TreeID& tid)
    {
        auto id = this->addVertex();
        mapping.insert({tid, id});
        stableMapping.insert({tid, this->getVertexProperty(id).first});
        return id;
    }

    InterconnectEdge<InterconnectGraph> connect(
        const utility::TreeID& a,
        const utility::TreeID& b
    )
    {
        auto id = this->addEdge(mapping[a], mapping[b]);
        return InterconnectEdge<InterconnectGraph>(id, this);
    }


    void mergeStarTopology( VertexID v )
    {
        auto inEdges  = this->getInEdges(v);
        auto outEdges = this->getOutEdges(v);

        std::list<std::tuple<VertexID, VertexID, Properties>> newEdges;
        for(auto inE = inEdges.first ; inE != inEdges.second ; ++inE)
        {
            auto inVertex = this->getEdgeSource(*inE);
            Properties inP = this->getEdgeProperty(*inE).second;

            for(auto outE = outEdges.first; outE != outEdges.second ; ++outE)
            {
                auto outVertex = this->getEdgeSource(*outE);
                if(inVertex == outVertex) continue;
                Properties outP = this->getEdgeProperty(*outE).second;

                auto newEdge = std::make_tuple(
                    inVertex,
                    outVertex,
                    mergeProperties( inP, outP )
                );
                newEdges.push_back( newEdge );
            }
        }

        for(auto& e : newEdges)
        {
            assert(std::get<0>(e) != std::get<1>(e)) ; // circular edges are not allowed
            this->addEdge(
                std::get<0>(e),
                std::get<1>(e),
                std::get<2>(e)
            );
        }

        this->removeVertex(v);
    }


    Properties mergeProperties(
        const Properties& a,
        const Properties& b
    ){
        Properties c;

        constexpr auto iter = hana::make_range(hana::int_c<0>, hana::int_c<std::tuple_size<Properties>::value-1>);
        hana::for_each(hana::to_tuple(iter),
            [&](auto i){
                auto ax = std::get<i>(a);
                auto bx = std::get<i>(b);
                std::get<i>(c) = ax.merge(bx);
            }
        );

        return c;
    }

};


} /* graph */
} /* dodo */
