#pragma once

#include <dout/dout.hpp>

#include <dodo/utility/tree_id.hpp>
#include <dodo/physical/attributes.hpp>
#include <dodo/graph/InterconnectGraph.hpp>

namespace dodo
{
namespace graph
{

template<typename... AttributeTypes>
class InterconnectGraph;

template<
    typename... T_Property
>
class HardwareGraphVertex
{
protected:
    dout::Dout& dout = dout::Dout::getInstance();
    utility::TreeID id;

    using ConsistsOfStructure = std::vector<HardwareGraphVertex>;

    std::shared_ptr<InterconnectGraph<T_Property...>> interconnectGraph;
    Property<T_Property...> localProperty;

    ConsistsOfStructure children;

    using InterconnectID = typename InterconnectGraph<T_Property...>::VertexID;
    using Mapping = std::map<utility::TreeID, InterconnectID, utility::TreeIDLess>;
    Mapping mapping;

public:

    HardwareGraphVertex(utility::TreeID i, std::shared_ptr<InterconnectGraph<T_Property...>> a) :
        id(i),
        interconnectGraph(a),
        localProperty(a->attributeMapStore)
    {
        mapping.insert({id, interconnectGraph->add()});
    }

    template<typename T_Child, typename... T_Args>
    utility::TreeID createChild(T_Args&&... args)
    {
        dout(dout::Flags::DEBUG) << "in createChild, id = " << id.get() << std::endl;
        auto consistID = id.genChildID();
        dout(dout::Flags::DEBUG) << "                consistId = " << consistID.get() << std::endl;

        T_Child child(consistID, interconnectGraph, std::forward(args)...);
        children.push_back(child);
        mapping.insert(child.mapping.begin(), child.mapping.end());

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

        dout(dout::Flags::INFO, offset==0) << id.get() << std::endl;
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