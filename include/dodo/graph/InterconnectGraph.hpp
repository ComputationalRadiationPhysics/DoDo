#pragma once

#include <tuple>

#include <boost/hana.hpp>

#include <dout/dout.hpp>

#include "BGL.hpp"
#include "InterconnectEdge.hpp"
#include "../utility/tree_id.hpp"


namespace dodo
{
namespace graph
{

template<typename T_InterconnectProperties>
class InterconnectGraph :
    public BGL<
        SimpleProperty,
        T_InterconnectProperties
    >
{
public:
    using TreeID = utility::TreeID;
    using Properties = T_InterconnectProperties;
    using Graph =  BGL<SimpleProperty, T_InterconnectProperties>;
    using VertexID = typename Graph::VertexID;
    using EdgeID = typename Graph::EdgeID;
    using StableVertexID = typename Graph::VertexPropertyBundle::first_type;
    using Mapping = std::map<TreeID, VertexID>;
    using StableMapping = std::map<TreeID, StableVertexID>;

    Mapping mapping;
    StableMapping stableMapping;

    VertexID add(const TreeID& tid)
    {
        const VertexID id{ this->addVertex() };
        mapping.insert({tid, id});
        stableMapping.insert({tid, this->getVertexProperty(id).first});
        return id;
    }

    InterconnectEdge<InterconnectGraph> connect(
        const TreeID& a,
        const TreeID& b
    )
    {
        const EdgeID id{ this->addEdge(mapping[a], mapping[b]) };
        return InterconnectEdge<InterconnectGraph>(id, this);
    }


    void mergeStarTopology(const VertexID v )
    {
        const auto inEdges  ( this->getInEdges(v) );
        const auto outEdges ( this->getOutEdges(v) );

        std::list<std::tuple<VertexID, VertexID, Properties>> newEdges;

        for(auto inE(inEdges.first) ; inE != inEdges.second ; ++inE)
        {
            const VertexID inVertex { this->getEdgeSource(*inE) };
            const Properties inP { this->getEdgeProperty(*inE).second };

            for(auto outE(outEdges.first); outE != outEdges.second ; ++outE)
            {
                const VertexID outVertex { this->getEdgeSource(*outE) };

                // discard circular edges
                if(inVertex == outVertex)
                    continue;

                const Properties outP { this->getEdgeProperty(*outE).second };

                newEdges.push_back(
                    std::make_tuple(
                        inVertex,
                        outVertex,
                        mergeProperties( inP, outP )
                    )
                );
            }
        }

        for(const auto& e : newEdges)
        {
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

        using namespace boost::hana;
        constexpr auto iter = make_range(
            int_c<0>,
            int_c< std::tuple_size<Properties>::value - 1 >
        );

        for_each(to_tuple(iter),
            [&a, &b, &c](const auto i){
                auto ax = std::get<i>(a);
                const auto bx = std::get<i>(b);
                std::get<i>(c) = ax.merge(bx);
            }
        );

        return c;
    }

};

} /* graph */
} /* dodo */
