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
    dout::Dout& dout = dout::Dout::getInstance();
    utility::TreeID id;
    std::shared_ptr<AttributeMapStore<T_Property...>> ams;

    using ConsistsOfStructure = std::vector<HardwareGraphVertex>;

protected:
    InterconnectGraph<T_Property...> interconnectGraph;
    Property<T_Property...> localProperty;

    ConsistsOfStructure children;

    using InterconnectID = typename InterconnectGraph<T_Property...>::VertexID;
    using Mapping = std::map<utility::TreeID, InterconnectID, utility::TreeIDLess>;
    Mapping mapping;

public:

    HardwareGraphVertex(utility::TreeID i, std::shared_ptr<AttributeMapStore<T_Property...>> a) :
        id(i),
        ams(a),
        interconnectGraph(a),
        localProperty(a)
    {
        mapping.insert({id, interconnectGraph.root});
    }

    template<typename T_Child, typename... T_Args>
    utility::TreeID createChild(T_Args&&... args)
    {
        dout(dout::Flags::DEBUG) << "in createChild, id = " << id.get() << std::endl;
        auto consistID = id.genChildID();
        dout(dout::Flags::DEBUG) << "                consistId = " << consistID.get() << std::endl;
        children.push_back(T_Child(consistID, ams, std::forward(args)...));

        auto commID = interconnectGraph.add();
        mapping.insert({consistID, commID});

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
