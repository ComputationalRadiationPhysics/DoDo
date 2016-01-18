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


    void mergeLinearPaths(
            VertexID a,
            VertexID v,
            VertexID b
            )
    {
        auto av = this->edgeRange(a, v);
        auto vb = this->edgeRange(v, b);

        // a->v + v->b
        std::list<Properties> ab;

        for(auto av1 = av.first; av1 != av.second; ++av1)
        {
            for(auto vb1 = vb.first; vb1 != vb.second; ++vb1)
            {
                ab.push_back(mergeProperties(
                    this->getEdgeProperty(*av1).second,
                    this->getEdgeProperty(*vb1).second)
                );
            }
        }
        for(auto& path : ab)
        {
            this->addEdge(a, b, path);
        }
    }

    Properties mergeProperties(const Properties& a, const Properties& b)
    {
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
