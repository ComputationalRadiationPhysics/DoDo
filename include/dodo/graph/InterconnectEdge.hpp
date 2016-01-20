#pragma once

namespace dodo
{
namespace graph
{

template<typename T_Graph, unsigned T_directions>
class InterconnectEdge
{

    T_Graph* graph;
    const typename T_Graph::EdgeID id1;
    const typename T_Graph::EdgeID id2;
public:

    InterconnectEdge(
        T_Graph* pAGraph,
        const typename T_Graph::EdgeID eid1,
        const typename T_Graph::EdgeID eid2
    ) :
        graph{pAGraph},
        id1{eid1},
        id2{eid2}
    {}

    template<typename T>
    T& getProperty()
    {
        T& t1 = this->graph->template getProperty<T>(id1);
        if(T_directions > 1)
            assert(t1 == this->graph->template getProperty<T>(id2));
        return t1;
    }

    template<typename T>
    void setProperty(const T t)
    {
        this->graph->template getProperty<T>(id1) = t;
        if(T_directions > 1)
            this->graph->template getProperty<T>(id2) = t;
    }

};



} /* graph */
} /* dodo */
