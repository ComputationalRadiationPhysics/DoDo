#pragma once

#include <tuple>
#include <algorithm>
#include <set>
#include <map>

#include <boost/hana.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/graph/graphml.hpp>

#include <dout/dout.hpp>

#include "dodo/graph/PairBGL.hpp"
#include "InterconnectEdge.hpp"
#include "dodo/utility/tree_id.hpp"
#include "dodo/utility/convert_tuple.hpp"


namespace dodo
{
namespace hardware
{

namespace hana = boost::hana;

template<typename T_EdgeProperties, typename T_VertexProperties = std::tuple<>>
class InterconnectGraph :
    public graph::PairBGL<
        T_VertexProperties,
        T_EdgeProperties
    >
{

public:
    using TreeID = utility::TreeID;
    using EdgeProperties = T_EdgeProperties;
    using VertexProperties = T_VertexProperties;
    using Graph =  graph::PairBGL<VertexProperties, EdgeProperties>;
    using VertexID = typename Graph::VertexID;
    using EdgeID = typename Graph::EdgeID;
    using StableVertexID = typename Graph::VertexPropertyBundle::first_type;
    using Mapping = std::map<TreeID, VertexID>;
    using StableMapping = std::map<TreeID, StableVertexID>;
    using EdgeHistory = std::set<EdgeID>;
    using EdgeHistoryMap = std::map<EdgeID, EdgeHistory>;
    using IndexMap = std::map<VertexID, size_t>;

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

    void mergeStarTopologyUndirected(const VertexID v, EdgeHistoryMap& edgeHistoryMap)
    {
        const auto adjacentVertices( this->getAdjacentVertices(v) );
        std::vector<decltype(this->edgeRange(v,v))> oldEdgeRanges;

        for(auto i=adjacentVertices.first ; i!=adjacentVertices.second ; ++i)
        {
            oldEdgeRanges.push_back(this->edgeRange(v, *i));
        }

        std::list<std::tuple<VertexID, VertexID, EdgeProperties, EdgeHistory>> newEdges;
        unsigned pos{0u};
        for(auto i=adjacentVertices.first ; i!=adjacentVertices.second ; ++i, ++pos)
        {
            const VertexID inV{*i};
            const auto inEdges = oldEdgeRanges[pos];

            auto j(i);
            ++j;
            for(auto pos2(pos+1); j!=adjacentVertices.second ; ++j, ++pos2)
            {
                const VertexID outV{*j};
                const auto outEdges = oldEdgeRanges[pos2];
                auto oldEdgesBetween(this->edgeRange(inV, outV));

                for(auto inE(inEdges.first) ; inE != inEdges.second ; ++inE)
                {
                    const EdgeProperties inP { this->getEdgeProperty(*inE).second };
                    EdgeHistory inEHist{edgeHistoryMap[*inE]};

                    for(auto outE(outEdges.first); outE != outEdges.second ; ++outE)
                    {
                        EdgeHistory newEHist{edgeHistoryMap[*outE]};
                        std::set_union(
                            inEHist.begin(), inEHist.end(),
                            newEHist.begin(), newEHist.end(),
                            std::inserter(newEHist,newEHist.begin())
                        );

                        bool hasBetterPath{ false };
                        for(auto e(oldEdgesBetween.first) ; !hasBetterPath && e!=oldEdgesBetween.second ; ++e)
                        {
                            EdgeHistory eh{ edgeHistoryMap[*e] };
                            hasBetterPath = std::includes(
                                newEHist.begin(), newEHist.end(),
                                eh.begin(), eh.end()
                            );

                            // dout::Dout& dout{ dout::Dout::getInstance()  };
                            // dout(dout::Flags::DEBUG) << "    spanningEdgeHistory: ";
                            // for(auto shx : eh)
                            // {
                            //     dout(dout::Flags::DEBUG, false) << "  " << shx;
                            // }
                            // dout(dout::Flags::DEBUG, false) << std::endl;
                            // dout(dout::Flags::DEBUG) << "    newEdgeHistory:      ";
                            // for(auto shx : newEHist)
                            // {
                            //     dout(dout::Flags::DEBUG, false) << "  " << shx;
                            // }
                            // dout(dout::Flags::DEBUG, false) << std::endl;
                        }
                        if(hasBetterPath) continue;

                        const EdgeProperties outP { this->getEdgeProperty(*outE).second };
                        newEdges.push_back(
                            std::make_tuple(inV, outV, mergeProperties(inP, outP), newEHist)
                        );
                    }
                }


            }
        }

        addNewEdges(newEdges, edgeHistoryMap);
        // for(auto i(0u) ; i<oldEdgeRanges.size() ; ++i)
        for(auto& i : oldEdgeRanges)
            deleteOldEdges(i, edgeHistoryMap);
        deleteOldVertex(v);
    }

    // For directed Graphs
    void mergeStarTopology(const VertexID v, EdgeHistoryMap& edgeHistoryMap)
    {

        const auto inEdges  ( this->getInEdges(v) );
        const auto outEdges ( this->getOutEdges(v) );

        std::list<std::tuple<VertexID, VertexID, EdgeProperties, EdgeHistory>> newEdges;

        for(auto inE(inEdges.first) ; inE != inEdges.second ; ++inE)
        {
            const VertexID inV { this->getEdgeSource(*inE) };
            const EdgeProperties inP { this->getEdgeProperty(*inE).second };
            EdgeHistory inEHist{edgeHistoryMap[*inE]};

            for(auto outE(outEdges.first); outE != outEdges.second ; ++outE)
            {
                const VertexID outV { this->getEdgeTarget(*outE) };
                if(inV == outV) continue; // discard circular edges

                EdgeHistory newEHist{edgeHistoryMap[*outE]};
                std::set_union(
                    inEHist.begin(), inEHist.end(),
                    newEHist.begin(), newEHist.end(),
                    std::inserter(newEHist,newEHist.begin())
                );

                bool hasBetterPath{ false };
                auto oldEdges(this->edgeRange(inV, outV));
                for(auto e(oldEdges.first) ; !hasBetterPath && e!=oldEdges.second ; ++e)
                {
                    EdgeHistory eh{ edgeHistoryMap[*e] };
                    hasBetterPath = std::includes(
                        newEHist.begin(), newEHist.end(),
                        eh.begin(), eh.end()
                    );

                    // dout::Dout& dout{ dout::Dout::getInstance()  };
                    // dout(dout::Flags::DEBUG) << "    spanningEdgeHistory: ";
                    // for(auto shx : eh)
                    // {
                    //     dout(dout::Flags::DEBUG, false) << "  " << shx;
                    // }
                    // dout(dout::Flags::DEBUG, false) << std::endl;
                    // dout(dout::Flags::DEBUG) << "    newEdgeHistory:      ";
                    // for(auto shx : newEHist)
                    // {
                    //     dout(dout::Flags::DEBUG, false) << "  " << shx;
                    // }
                    // dout(dout::Flags::DEBUG, false) << std::endl;
                }
                if(hasBetterPath) continue;

                const EdgeProperties outP { this->getEdgeProperty(*outE).second };
                newEdges.push_back(
                    std::make_tuple(inV, outV, mergeProperties(inP, outP), newEHist)
                );
            }
        }

        addNewEdges(newEdges, edgeHistoryMap);
        deleteOldEdges(inEdges, edgeHistoryMap);
        deleteOldEdges(outEdges, edgeHistoryMap);
        deleteOldVertex(v);

    }

    void deleteOldVertex(VertexID v)
    {
        this->removeVertex(v);
    }

    void addNewEdges(std::list<std::tuple<VertexID, VertexID, EdgeProperties, EdgeHistory>> nEdges, EdgeHistoryMap& hMap)
    {
        for(const auto& e : nEdges)
        {
            auto newid = this->addEdge(std::get<0>(e), std::get<1>(e), std::get<2>(e));
            hMap[newid] = {std::get<3>(e)};
        }
    }

    template<typename T>
    void deleteOldEdges(std::pair<T,T> iterators, EdgeHistoryMap& hMap)
    {
        for(auto e(iterators.first); e != iterators.second ; ++e)
        {
            this->removeEdge(*e);
            hMap.erase(*e);
        }
    }


    /**
     * This method is not intended to be used directly.
     * Should only be called from the friend class InterconnectEdge
     */
    template<typename T>
    T& getProperty(const EdgeID& e)
    {
        constexpr int tupleIndex { utility::tuple_index<EdgeProperties, T>::value  };
        static_assert(tupleIndex >= 0, "This property does not exist");
        EdgeProperties& properties = this->getEdgeProperty(e).second;
        return std::get<tupleIndex>(properties);
    }

    template<typename T>
    T& getProperty(const VertexID& e)
    {
        constexpr int tupleIndex { utility::tuple_index<VertexProperties, T>::value  };
        static_assert(tupleIndex >= 0, "This property does not exist");
        VertexProperties& properties = this->getVertexProperty(e).second;
        return std::get<tupleIndex>(properties);
    }

private:
    template<typename Properties>
    Properties mergeProperties(
        const Properties& a,
        const Properties& b
    ){
        Properties c;

        using namespace boost::hana;
        constexpr auto iter = make_range(
            int_c<0>,
            int_c< std::tuple_size<Properties>::value >
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

public:

    // void printEdgeHistory(std::string offset="")
    // {
    //     dout::Dout& dout{ dout::Dout::getInstance()  };

    //     dout(dout::Flags::DEBUG) << offset << "HistoryMap:" <<  std::endl;
    //     for(auto& histkey : edgeHistoryMap)
    //     {
    //         dout(dout::Flags::DEBUG) << offset << " " << histkey.first << ":";
    //         for(auto& hist : histkey.second)
    //             dout(dout::Flags::DEBUG, false) << " " << hist;
    //         dout(dout::Flags::DEBUG, false) << std::endl;

    //     }

    // }

};


namespace detail
{

template<typename T_Property, typename T, typename U, typename X>
auto
genericPrinter(std::shared_ptr<InterconnectGraph<T, U>> g, X allEoV)
{
    //using Graph = InterconnectGraph<T, U>;
    using Key = typename std::decay<decltype(*X::first)>::type;
    //using Map = std::map<Key, decltype(((T_Property*)nullptr)->toString())>;
    using Map = std::map<Key, std::string>;
    Map m;
    for (auto i = allEoV.first; i != allEoV.second; ++i)
    {
        m[*i] = g->template getProperty<T_Property>(*i).toString();
    }
    return m;
}


template<typename T>
void addToDynamicProperties(boost::dynamic_properties &dp, T &maps)
{
    for (unsigned i{0}; i < maps.size(); ++i)
    {
        dp.property(
            maps[i].first,
            boost::associative_property_map<decltype(maps[i].second)>(maps[i].second)
        );
    }
}


template<typename T, typename U, typename T_Labels, typename X>
auto
getMaps(std::shared_ptr<InterconnectGraph<T, U>> g, T_Labels labels, X vertexOrEdgeIts)
{
    // using Graph = InterconnectGraph<T, U>;
    using namespace std;
    using Key = typename decay<decltype(*X::first)>::type;
    vector<pair<string, map<Key, string>>> maps(tuple_size<T_Labels>::value);
    int index = 0;
    hana::for_each(
        hana::to_tuple(labels),
        [&index, g, &maps, vertexOrEdgeIts](auto label)
        {
            using LabelType = typename decay<decltype(get<1>(label))>::type;
            auto currentMap = genericPrinter<LabelType>(g, vertexOrEdgeIts);
            maps[index++] = (make_pair(get<0>(label), currentMap));
        }
    );
    return maps;
}

} /* namespace detail */

template<typename T, typename U, typename T_VertexLabelTuple, typename T_EdgeLabelTuple>
void writeGraph(
    std::shared_ptr<InterconnectGraph<T, U>>& g,
    T_VertexLabelTuple vertexLabels,
    T_EdgeLabelTuple edgeLabels,
    std::ostream& stream = std::cout
){

    using namespace std;
    using Graph = InterconnectGraph<T, U>;
    // auto& dout = dout::Dout::getInstance();
    boost::dynamic_properties dp;
    using IndexMap = typename Graph::IndexMap;
    IndexMap im;
    boost::associative_property_map<IndexMap> propmapIndex(im);

    auto allV = g->getVertices();
    int index=0;
    for(auto i=allV.first ; i!=allV.second ; ++i)
    {
        propmapIndex[*i] = index++;
    }

    auto edgeMaps = detail::getMaps(g, edgeLabels, g->getEdges());
    auto vertexMaps = detail::getMaps(g, vertexLabels, g->getVertices());

    detail::addToDynamicProperties(dp, edgeMaps);
    detail::addToDynamicProperties(dp, vertexMaps);

    boost::write_graphml(stream, *(g->graph), propmapIndex, dp, false);
}


template<typename T, typename U>
typename InterconnectGraph<T, U>::EdgeHistoryMap
initEdgeHistory(std::shared_ptr<InterconnectGraph<T, U>> g)
{
    typename InterconnectGraph<T, U>::EdgeHistoryMap edgeHistoryMap;
    auto allEdges = g->getEdges();
    for(auto e(allEdges.first) ; e != allEdges.second; ++e )
    {
        edgeHistoryMap[*e] = {*e};
    }
    return edgeHistoryMap;
}




template<typename T, typename T2, typename T_Bundle>
struct TupleTransformCopier
{
    using OldGraph = typename T::BGLGraph;
    using NewGraph = typename T2::BGLGraph;

    typename boost::property_map<OldGraph, T_Bundle>::const_type all_map1;
    mutable typename boost::property_map<NewGraph, T_Bundle>::type       all_map2;

    TupleTransformCopier(const OldGraph& g1, NewGraph& g2, T_Bundle bundle) :
        all_map1(boost::get(bundle, g1)),
        all_map2(boost::get(bundle, g2))
    {}

    template<typename Elem>
    void operator()(const Elem& e1, Elem& e2)
    {
        //should be a pair <unsigned, std::tuple<Properties...>>
        auto prop1 = all_map1[e1];
        auto prop2 = all_map2[e2];

        prop2.first = prop1.first;
        utility::convertTuple(prop1.second, prop2.second);
        put(all_map2, e2, prop2);
    }
};

struct NonCopier
{
    template<typename Elem>
    void operator()(const Elem&, Elem&)
    {}
};

template<
    typename T_Combined,
    typename T_OldInterconnect_t,
    typename T_IsoMap
>
auto copyAndTransform(std::shared_ptr<T_OldInterconnect_t> interconnectGraph, T_IsoMap& isoMap)
{

    boost::associative_property_map<T_IsoMap> mapWrapper{isoMap};
    auto reducedIGraph( std::make_shared<T_Combined>() );

    TupleTransformCopier<
         T_OldInterconnect_t,
         T_Combined,
         boost::edge_bundle_t
    >  ec(*(interconnectGraph->graph), *(reducedIGraph->graph), boost::edge_bundle);

    NonCopier vc;

    boost::copy_graph(
        *(interconnectGraph->graph),
        *(reducedIGraph->graph),
        boost::orig_to_copy(mapWrapper).edge_copy(ec).vertex_copy(vc));

    return reducedIGraph;

}




} /* graph */
} /* dodo */
