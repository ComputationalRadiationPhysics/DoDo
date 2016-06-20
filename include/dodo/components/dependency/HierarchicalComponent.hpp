#pragma once

#include <utility>
#include <numeric>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/mpl/range_c.hpp>

#include <dodo/components/dependency/ComponentBase.hpp>
#include <dodo/components/types/PortVertexContainer.hpp>


namespace dodo{
namespace components{
namespace dependency{

    struct HierarchicalBase :
        public ComponentBase
    {
        using Subgraph =  ComponentBase::Subgraph;
        using SubcomponentMap = std::map< std::string, ComponentBase* >;
        SubcomponentMap subcomponents;

        HierarchicalBase(
            Subgraph & parentGraph,
            std::string name,
            std::vector<std::string> inNames,
            std::vector<std::string> outNames
        ) :
            ComponentBase(
                parentGraph,
                name,
                inNames,
                outNames
            )
        {
            boost::get_property(sg, boost::graph_graph_attribute)["style"] = "dotted";
            for( auto i : inPorts )
            {
                sg[i.second] = ::types::PortVertexTypes::HIERARCHICAL_IN;
            }
            for( auto i : outPorts )
            {
                sg[i.second] = ::types::PortVertexTypes::HIERARCHICAL_OUT;
            }
        }

        template<
    typename T_Comp,
    typename... T_Args
    >
        SubcomponentMap::mapped_type
        createSubcomponent(std::string name, T_Args && ... args )
        {
            SubcomponentMap::mapped_type comp{ new T_Comp(
                sg,
                this->name + "." + name,
                std::forward< T_Args >( args )...
            )};
            subcomponents[name] = comp;
            return comp;
        }



        ~HierarchicalBase( ) override
        {
            for(auto i : subcomponents)
            {
                delete i.second;
            }
        }

        Subgraph::vertex_descriptor
        findVertexFromString( std::string s ) override
        {
            auto pos = s.find('.');
            if(pos == std::string::npos)
            {
                // one of our own ports
                return ComponentBase::findVertexFromString(s);
            }else{
                // port of a subcomponent
                std::string sub = s.substr(0,pos);
                std::string v = s.substr(pos+1);
                return subcomponents[sub] -> findVertexFromString(v);
            }

        }


        template<typename T_NewSub>
        void replaceSubcomponentWith(
            std::string subName,
            std::map< std::string, std::string > InPortMapping,
            std::map< std::string, std::string > OutPortMapping
        )
        {
            auto oldComp = subcomponents.at(subName);
            auto newComp = createSubcomponent<T_NewSub>(subName);

            // TODO Find all edges in the current component that lead to inPorts of oldComp
            // casts to prevent unused parameter warning
            (void) InPortMapping;
            (void) OutPortMapping;
            // For each of these edges, reconnect them to the InPorts of newComp
            // according to the portMapping

            // Repeat for OutPortMapping

            throw std::runtime_error( "Method not finished!" );

            delete oldComp;
        }



    };

}
}
}

