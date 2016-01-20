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

namespace hana = boost::hana;

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
    using Graph =  BGL<SimpleProperty, Properties>;
    using VertexID = typename Graph::VertexID;
    using EdgeID = typename Graph::EdgeID;
    using StableVertexID = typename Graph::VertexPropertyBundle::first_type;
    using Mapping = std::map<TreeID, VertexID>;
    using StableMapping = std::map<TreeID, StableVertexID>;

    template<unsigned T, typename U> friend class InterconnectEdge;

    Mapping mapping;
    StableMapping stableMapping;

    VertexID add(const TreeID& tid)
    {
        const VertexID id{ this->addVertex() };
        mapping.insert({tid, id});
        stableMapping.insert({tid, this->getVertexProperty(id).first});
        return id;
    }

    template<unsigned T_directions>
    InterconnectEdge<T_directions, InterconnectGraph> connect(
        const TreeID& a,
        const TreeID& b
    ){
        return connectImpl(hana::int_c<T_directions>, a, b);
    }

    void mergeStarTopology(const VertexID v )
    {
        dout::Dout& dout{ dout::Dout::getInstance()  };

        const auto inEdges  ( this->getInEdges(v) );
        const auto outEdges ( this->getOutEdges(v) );

        dout(dout::Flags::DEBUG) << "    InEdges: " << std::distance(inEdges.first, inEdges.second) << std::endl;
        dout(dout::Flags::DEBUG) << "    OutEdges: " << std::distance(outEdges.first, outEdges.second) << std::endl;

        std::list<std::tuple<VertexID, VertexID, Properties>> newEdges;

        for(auto inE(inEdges.first) ; inE != inEdges.second ; ++inE)
        {
            const VertexID inVertex { this->getEdgeSource(*inE) };
            const Properties inP { this->getEdgeProperty(*inE).second };
            std::cout << std::get<1>(this->getEdgeProperty(*inE).second).value << std::endl;

            for(auto outE(outEdges.first); outE != outEdges.second ; ++outE)
            {
                const VertexID outVertex { this->getEdgeTarget(*outE) };

                // discard circular edges
                if(inVertex == outVertex)
                    continue;

                const Properties outP { this->getEdgeProperty(*outE).second };
                std::cout << std::get<1>(this->getEdgeProperty(*outE).second).value << std::endl;

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
            // TODO: only add edges, if no other edge exists?
            this->addEdge(
                std::get<0>(e),
                std::get<1>(e),
                std::get<2>(e)
            );
            dout(dout::Flags::DEBUG) << "    adding Edge " << std::get<0>(e) << " -> " << std::get<1>(e) << std::endl;
        }

        for(auto inE(inEdges.first) ; inE != inEdges.second ; ++inE)
            this->removeEdge(*inE);
        for(auto outE(outEdges.first); outE != outEdges.second ; ++outE)
            this->removeEdge(*outE);
        this->removeVertex(v);

    }

private:

    /**
     * This method is not intended to be used directly.
     * Should only be called from the friend class InterconnectEdge
     */
    template<typename T>
    T& getProperty(const EdgeID& e)
    {
        constexpr size_t tupleIndex { utility::tuple_index<Properties, T>::value  };
        static_assert(static_cast<int>(tupleIndex) >= 0);
        Properties properties = this->getEdgeProperty(e).second;
        return std::get<tupleIndex>(properties);
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

        for_each(
            to_tuple(iter),
            [&a, &b, &c](const auto i){
                auto ax = std::get<i>(a);
                const auto bx = std::get<i>(b);
                std::get<i>(c) = ax.merge(bx);
            }
        );

        return c;
    }


    InterconnectEdge<1, InterconnectGraph> connectImpl(
        hana::int_<1>,
        const TreeID& a,
        const TreeID& b
    ){
        const EdgeID id1{ this->addEdge(mapping[a], mapping[b]) };
        return InterconnectEdge<1, InterconnectGraph>(this, id1);
    }

    InterconnectEdge<2, InterconnectGraph> connectImpl(
        hana::int_<2>,
        const TreeID& a,
        const TreeID& b
    ){
        const EdgeID id1{ this->addEdge(mapping[a], mapping[b]) };
        const EdgeID id2{ this->addEdge(mapping[b], mapping[a]) };
        return InterconnectEdge<2, InterconnectGraph>(this, id1, id2);
    }

};

} /* graph */
} /* dodo */
