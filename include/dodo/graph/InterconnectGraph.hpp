#pragma once

#include <tuple>
#include <algorithm>
#include <set>
#include <map>

#include <boost/hana.hpp>
#include <boost/graph/graphml.hpp>

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
        T_InterconnectProperties,
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

    void mergeStarTopology(const VertexID v, EdgeHistoryMap& edgeHistoryMap)
    {

        const auto inEdges  ( this->getInEdges(v) );
        const auto outEdges ( this->getOutEdges(v) );

        std::list<std::tuple<VertexID, VertexID, Properties, EdgeHistory>> newEdges;

        for(auto inE(inEdges.first) ; inE != inEdges.second ; ++inE)
        {
            const VertexID inV { this->getEdgeSource(*inE) };
            const Properties inP { this->getEdgeProperty(*inE).second };
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

                    dout::Dout& dout{ dout::Dout::getInstance()  };
                    dout(dout::Flags::DEBUG) << "    spanningEdgeHistory: ";
                    for(auto shx : eh)
                    {
                        dout(dout::Flags::DEBUG, false) << "  " << shx;
                    }
                    dout(dout::Flags::DEBUG, false) << std::endl;
                    dout(dout::Flags::DEBUG) << "    newEdgeHistory:      ";
                    for(auto shx : newEHist)
                    {
                        dout(dout::Flags::DEBUG, false) << "  " << shx;
                    }
                    dout(dout::Flags::DEBUG, false) << std::endl;
                }
                if(hasBetterPath) continue;

                const Properties outP { this->getEdgeProperty(*outE).second };
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

    void addNewEdges(std::list<std::tuple<VertexID, VertexID, Properties, EdgeHistory>> nEdges, EdgeHistoryMap& hMap)
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
        constexpr size_t tupleIndex { utility::tuple_index<Properties, T>::value  };
        static_assert(static_cast<int>(tupleIndex) >= 0, "This property does not exist");
        Properties& properties = this->getEdgeProperty(e).second;
        return std::get<tupleIndex>(properties);
    }

    template<typename T>
    T& getProperty(const VertexID& e)
    {
        constexpr size_t tupleIndex { utility::tuple_index<Properties, T>::value  };
        static_assert(static_cast<int>(tupleIndex) >= 0, "This property does not exist");
        Properties& properties = this->getVertexProperty(e).second;
        return std::get<tupleIndex>(properties);
    }

private:
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
template<typename T, typename T_Property>
struct VertexPrinter
{
    using Graph = InterconnectGraph<T>;
    std::shared_ptr<Graph> g;
    using Map = std::map<typename Graph::VertexID, std::string>;
    Map m;
    boost::associative_property_map<Map> propMap;

    VertexPrinter(std::shared_ptr<Graph> g) :
        g(g),
        propMap(m)
    {
        auto allE = g->getVertices();
        for(auto i=allE.first ; i!=allE.second ; ++i)
        {
            m[*i] = g->template getProperty<T_Property>(*i).toString();
        }

    }
};







// template<typename T, typename T_Property>
// struct EdgePrinter
// {
//     using Graph = InterconnectGraph<T>;
//     std::shared_ptr<Graph> g;
//     using Map = std::map<typename Graph::EdgeID, std::string>;
//     Map m;
//     boost::associative_property_map<Map> propMap;

//     EdgePrinter(std::shared_ptr<Graph> g) :
//         g(g)
//         , propMap(m)
//     {
//         auto allE = g->getEdges();
//         for(auto i=allE.first ; i!=allE.second ; ++i)
//         {
//             m[*i] = g->template getProperty<T_Property>(*i).toString();
//         }

//     }
// };

// template<typename T, typename T_Property>
// auto
// edgePrinter(std::shared_ptr<InterconnectGraph<T>> g)
// {
//     using Graph = InterconnectGraph<T>;
//     using Map = std::map<typename Graph::EdgeID, std::string>;
//     Map m;
//     auto allE = g->getEdges();
//     for(auto i=allE.first ; i!=allE.second ; ++i)
//     {
//         m[*i] = g->template getProperty<T_Property>(*i).toString();
//     }
//     return m;
// }

template<typename T, typename T_Property, typename V, typename X>
auto
genericPrinter(std::shared_ptr<InterconnectGraph<T>> g, X allEoV)
{
    using Graph = InterconnectGraph<T>;
    using Map = std::map<V, std::string>;
    Map m;
    //auto allE = g->getVertices();
    for(auto i=allEoV.first ; i!=allEoV.second ; ++i)
    {
        m[*i] = g->template getProperty<T_Property>(*i).toString();
    }
    return m;
}

template<typename T>
void addToDynamicProperties(boost::dynamic_properties& dp, T& maps)
{
    for(unsigned i{0} ; i<maps.size() ; ++i)
    {
        dp.property(
            maps[i].first,
            boost::associative_property_map<decltype(maps[i].second)>(maps[i].second)
        );
    }
}

template<typename T, typename T_VertexLabelTuple, typename T_EdgeLabelTuple>
void writeGraph(
    std::shared_ptr<InterconnectGraph<T>>& g,
    T_VertexLabelTuple vertexLabels,
    T_EdgeLabelTuple edgeLabels,
    std::ostream& stream = std::cout
){

    using namespace std;
    using Graph = InterconnectGraph<T>;
    auto& dout = dout::Dout::getInstance();
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

    vector<pair<string, map<typename Graph::EdgeID, string>>> edgeMaps(std::tuple_size<T_EdgeLabelTuple>::value);
    index=0;
    hana::for_each(
        hana::to_tuple(edgeLabels),
        [&index, g, &edgeMaps](auto label)
        {
            using LabelType = typename decay<decltype(get<1>(label))>::type;
            auto currentMap = genericPrinter<T, LabelType, typename Graph::EdgeID>(g, g->getEdges());
            edgeMaps[index++] = (make_pair(get<0>(label), currentMap));
        }
    );

    vector<pair<string, map<typename Graph::VertexID, string>>> vertexMaps(std::tuple_size<T_VertexLabelTuple>::value);
    index=0;
    hana::for_each(
        hana::to_tuple(vertexLabels),
        [&index, g, &vertexMaps](auto label)
        {
            using LabelType = typename decay<decltype(get<1>(label))>::type;
            auto currentMap = genericPrinter<T, LabelType, typename Graph::VertexID>(g, g->getVertices());
            vertexMaps[index++] = (make_pair(get<0>(label), currentMap));
        }
    );

    addToDynamicProperties(dp, edgeMaps);
    addToDynamicProperties(dp, vertexMaps);

    boost::write_graphml(stream, *(g->graph), propmapIndex, dp, false);

}


template<typename T>
typename InterconnectGraph<T>::EdgeHistoryMap
initEdgeHistory(std::shared_ptr<InterconnectGraph<T>> g)
{
    typename InterconnectGraph<T>::EdgeHistoryMap edgeHistoryMap;
    auto allEdges = g->getEdges();
    for(auto e(allEdges.first) ; e != allEdges.second; ++e )
    {
        edgeHistoryMap[*e] = {*e};
    }
    return edgeHistoryMap;
}



} /* graph */
} /* dodo */
