#include <iostream>
#include <boost/graph/graphml.hpp>
#include <dodo/components/dependency/HierarchicalComponent.hpp>
#include <dodo.hpp>


using namespace dodo::components::dependency;

struct AtomicCombine :
    public AtomicBase
{
    AtomicCombine( AtomicBase::Subgraph & parentGraph, std::string name ) :
        AtomicBase(
            parentGraph,
            name,
            {"in1", "in2"},
            {"out1"}
        )
    {
    }
};

struct AtomicProcessing :
    public AtomicBase
{
    AtomicProcessing( AtomicBase::Subgraph & parentGraph, std::string name) :
        AtomicBase(
            parentGraph,
            name,
            {"in1"},
            {"out1"}
        )
    {
    }
};

struct AtomicDuplicate :
    public AtomicBase
{
    AtomicDuplicate( AtomicBase::Subgraph & parentGraph, std::string name) :
        AtomicBase(
            parentGraph,
            name,
            {"in1"},
            {"out1","out2"}

        )
    { }
};



int main( )
{
    using SG = ComponentBase::Subgraph;
    SG sg;
    TemplatePipe<AtomicProcessing, AtomicDuplicate, AtomicCombine> hc (sg, "TempPipe");
    std::stringstream ss;
    boost::dynamic_properties dp;
   dp.property("PortType", get(&types::PortVertexContainer::t , sg));
    boost::write_graphml(
        ss,
        sg
        ,dp
    );
    std::cout << ss.str( ) << std::endl;

    return 0;
}
