#pragma once

#include <dout/dout.hpp>

#include <dodo/utility/tree_id.hpp>
#include <dodo/physical/attributes.hpp>
#include <dodo/graph/InterconnectGraph.hpp>

namespace dodo
{
namespace graph
{


template<
    typename T_LocalProperties,
    typename T_InterconnectProperties
>
class HardwareGraphVertex
{
protected:
    dout::Dout& dout = dout::Dout::getInstance();
    utility::TreeID id;

    using ConsistsOfStructure = std::vector<HardwareGraphVertex>;
    using InterconnectGraph_T = InterconnectGraph<T_InterconnectProperties>;

    std::shared_ptr<InterconnectGraph_T> interconnectGraph;
    T_LocalProperties properties;

    ConsistsOfStructure children;

    using InterconnectID = typename InterconnectGraph_T::VertexID;
    using Mapping = std::map<utility::TreeID, InterconnectID, utility::TreeIDLess>;

public:

    HardwareGraphVertex(utility::TreeID i, std::shared_ptr<InterconnectGraph_T> a) :
        id(i),
        interconnectGraph(a)
    {
        interconnectGraph->add(id);
    }

    template<typename T_Child, typename... T_Args>
    utility::TreeID createChild(T_Args&&... args)
    {
        dout(dout::Flags::DEBUG) << "in createChild, id = " << id.get() << std::endl;
        auto consistID = id.genChildID();
        dout(dout::Flags::DEBUG) << "                consistId = " << consistID.get() << std::endl;

        T_Child child(consistID, interconnectGraph, std::forward(args)...);
        children.push_back(child);

        return consistID;
    }

    void printLocalChildren()
    {
        dout(dout::Flags::INFO) << id.get() << std::endl;
        for(const auto& c : children)
        {
            dout(dout::Flags::INFO) << "\\___" << c.id.get() << std::endl;
        }
    }

    void printAllChildren(const size_t offset = 0) const
    {
        std::stringstream indent;
        for(auto i=0u ; i<offset+id.get().size() ; ++i)
            indent << " ";

        dout(dout::Flags::INFO, offset==0) << id.get() << " ---> " << interconnectGraph->mapping.at(id) << std::endl;
        for(unsigned i=0; i<children.size(); ++i)
        {
            dout(dout::Flags::INFO) << indent.str() << "\\___";
            children[i].printAllChildren(offset + 4);
            if(i+1 == children.size())
                dout(dout::Flags::INFO) << std::endl;
        }

    }

};




} /* graph */
} /* dodo */
