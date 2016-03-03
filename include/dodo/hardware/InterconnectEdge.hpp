#pragma once


#include <boost/hana.hpp>

namespace dodo
{
namespace hardware
{

template<unsigned T_directions, typename T_Graph>
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

    InterconnectEdge(
        T_Graph* pAGraph,
        const typename T_Graph::EdgeID eid1
    ) :
        graph{pAGraph},
        id1{eid1}
    {}

    template<typename T>
    T getProperty()
    {
        return this->graph->template getProperty<T>(id1);
    }

    template<typename T>
    void setProperty(const T t)
    {
        setPropertyImpl(boost::hana::int_c<T_directions>, t);
    }

private:

    template<typename T>
    void setPropertyImpl(boost::hana::int_<1>, const T t)
    {
        this->graph->template getProperty<T>(id1) = t;
    }

    template<typename T>
    void setPropertyImpl(boost::hana::int_<2>, const T t)
    {
        this->graph->template getProperty<T>(id1) = t;
        this->graph->template getProperty<T>(id2) = t;
    }

};


} /* graph */
} /* dodo */
