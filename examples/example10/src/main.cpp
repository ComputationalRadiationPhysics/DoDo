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

    std::set<SG::vertex_descriptor> hidden_vertices;
    using Filtered = boost::filtered_graph<SG, boost::keep_all, std::function<bool(SG::vertex_descriptor)> >;
    Filtered f(sg, boost::keep_all{}, [&](auto v){ return hidden_vertices.find(v)==hidden_vertices.end();});



    // Access like this if it is a BundledProperty!
    dp.property("PortType", get(&types::PortVertexContainer::t , sg));
    // Access like this if it is an InternalProperty
    dp.property("VertexIndex", get(boost::vertex_index, sg));

    boost::write_graphml( ss, f,dp );
    //    boost::write_graphviz(ss, sg);
    std::cout << ss.str( ) << std::endl;
    std::cout << boost::distance(boost::vertices(f)) << std::endl;
    std::stringstream().swap(ss);

    std::ofstream ofs;
    ofs.open("/tmp/g.dot");
    boost::write_graphviz(ofs, sg);
    ofs.close();


    hidden_vertices.insert(12);
    boost::write_graphml( ss, f,dp );
    std::cout << ss.str( ) << std::endl;
    std::cout << boost::distance(boost::vertices(f)) << std::endl;

    return 0;
}
