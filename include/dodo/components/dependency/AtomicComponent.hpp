#pragma once


#include <string>

#include <dodo/components/dependency/ComponentBase.hpp>
#include <dodo/components/types/PortVertexContainer.hpp>


namespace dodo{
namespace components{
namespace dependency{

    struct AtomicBase :
        public ComponentBase
    {
        using Subgraph = ComponentBase::Subgraph;

        // local ID
        std::pair<std::string, Subgraph::vertex_descriptor> step;
//        std::map

        AtomicBase(
            Subgraph & parentGraph,
            std::string name,
            std::vector <std::string> inNames,
            std::vector <std::string> outNames
        ) :
            ComponentBase(
                parentGraph,
                name,
                inNames,
                outNames
            ),
            step { std::make_pair( "step", add_vertex( sg ) ) }
        {
            boost::get_property(sg, boost::graph_graph_attribute)["style"] = "filled";

            auto pm = get(boost::vertex_attribute, sg);
            pm[step.second]["shape"] = "square";
            pm[step.second]["label"] = step.first;
            sg[step.second] = types::PortVertexTypes::ATOMIC_STEP;

            for( auto i : inPorts )
            {
                sg[i.second] = types::PortVertexTypes::ATOMIC_IN;
                add_edge( i.second, step.second, sg );
            }

            for( auto i : outPorts )
            {
                sg[i.second] = types::PortVertexTypes::ATOMIC_OUT;
                add_edge( step.second, i.second, sg );
            }


        }
    };

}
}
}
