#pragma once

#include "DependencyBGL.hpp"
#include "ComponentMeta.hpp"
#include "CommunicationMeta.hpp"

namespace dodo
{
namespace process
{


template<typename T_ComponentMeta>
struct ComponentMetaVertexWrapper
{
    using VertexID = SimpleBGL<ComponentMeta, CommunicationMeta>::VertexID;
    VertexID id;

    std::pair<VertexID, enum T_ComponentMeta::OutNames>
    port(enum T_ComponentMeta::OutNames p)
    {
        auto r = std::make_pair(id, p);
        return r;
    }

    std::pair<VertexID, enum T_ComponentMeta::InNames>
    port(enum T_ComponentMeta::InNames p)
    {
        auto r = std::make_pair(id, p);
        return r;
    }

    auto
    port(unsigned p)
    {
        auto r = std::make_pair(id, p);
        return r;
    }

};


class DependencyGraph :
    public SimpleBGL<ComponentMeta, CommunicationMeta>
{
public:
    using BGL = SimpleBGL<ComponentMeta, CommunicationMeta>;
    using VertexID = BGL::VertexID;
    using EdgeID = BGL::EdgeID;

    template<typename T_Component, typename... Args>
    auto create(T_Component c)
    {
//        T_Component(std::forward<Args...>(args)...) component;
//        VertexID v = addVertex();
//        component.vertexID = v;

        return ComponentMetaVertexWrapper<T_Component>{addVertex(c)};
    }

    template<typename T_FromPort, typename T_ToPort>
    EdgeID createDependency(VertexID fromV, T_FromPort fromP, VertexID toV, T_ToPort toP)
    {
        auto e = addEdge(fromV, toV, {fromP, toP});
        return e;
    }

    template<typename T_FromPort, typename T_ToPort>
    EdgeID createDependency(T_FromPort fromP, T_ToPort toP)
    {
        auto e = addEdge(fromP.first, toP.first, {fromP.second, toP.second});
        return e;
    }


};


} /* process */
} /* dodo */

