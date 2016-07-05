#include <iostream>
#include <boost/graph/graphml.hpp>
#include <boost/graph/grid_graph.hpp>
#include <numeric>
#include <array>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <dodo/graph/SimpleBGL.hpp>


//struct ComputeComponent
//{
//    std::vector<InPorts*> inPorts;
//    std::vector<OutPorts*> outPorts;
//    virtual void operator()();
//};


//template<int Dimension=2, int nCells = 3>
//struct BorderMemory
//{
//    std::array<int, nCells*nCells> mem;

//    // convention: dim==0 means dim is not active
//    //             dim==-1 means lower end
//    //             dim== 1 means higher end
//    std::array<int, nCells> getBorder(int dim1, int dim2)
//    {
//        // There must be exactly 1 active dim at all times
//        assert(abs(dim1)+abs(dim2) == 1);
//        std::array<int, nCells> resultingBorder;

//        // North
//        if(dim1 == -1)
//            for(int i=0 ; i<nCells; ++i)
//                resultingborder[i]=mem[0][i];

//        // South
//        if(dim1 == 1)
//            for(int i=0 ; i<nCells; ++i)
//                resultingborder[i] = mem[nCells-1][i];

//        // East
//        if(dim2 == 1)
//            for(int i=0 ; i<nCells; ++i)
//                resultingborder[i] = mem[i][nCells-1];

//        // West
//        if(dim2 == -1)
//            for(int i=0 ; i<nCells; ++i)
//                resultingborder[i] = mem[i][0];

//        return resultingBorder;
//    }

//    int get(int i, int j)
//    {
//        return mem[i][j];
//    }

//};

//template<int Dimension=2, int nCells = 3>
//struct GoLKernel : ComputeComponent
//{
//    // inPorts:
//    // 0 -> own state
//    // 1 -> North
//    // 2 -> East
//    // 3 -> South
//    // 4 -> West

//    // TODO: generate based on Dimension
//    std::array<int, (2+nCells)*(2+nCells)> states;
//    BorderMemory<Dimension, nCells> statesNew;

//    int collectState(int i, int j, const std::array<int, nCells*nCells>& states)
//    {
//        int count = 0;
//        count += guardedAccess(i-1, j-1);
//        count += guardedAccess(i+1, j-1);
//        count += guardedAccess(i-1, j+1);
//        count += guardedAccess(i+1, j+1);
//        return count;
//    }

//    int guardedAccess(int i, int j)
//    {
//        if( i >= 0 && i < nCells && j >= 0 && j < nCells )
//            return inPorts[0]->getData().get(i,j);
//        if( i == -1 )
//            return inPorts[1]->getData( )[j];
//        if( i == nCells )
//            return inPorts[2]->getData( )[j];
//        if( j == -1 )
//            return inPorts[4]->getData( )[i];
//        if( j == nCells )
//            return inPorts[3]->getData( )[i];
//    }

//    void writeInternalState(BorderMemory<Dimension, nCells>& statesNew)
//    {
//        outPorts[0]->putData(statesNew.mem);
//        outPorts[1]->putData(statesNew.getBorder(-1,0));
//        outPorts[2]->putData(statesNew.getBorder(0,1));
//        outPorts[3]->putData(statesNew.getBorder(1,0));
//        outPorts[4]->putData(statesNew.getBorder(0,-1));
//    }

//    virtual void operator()()
//    {
//        for(int i=0; i<nCells; ++i)
//        {
//            for( int j = 0; j < nCells; ++j )
//            {
//                statesNew[i][j] = collectState( i, j, states);
//            }
//        }
//        writeInternalState(statesNew);
//    }

//};

struct VProp
{
    size_t id;
    int x;
    int y;
    float coefficient;
    float workload;
    std::string coordinatestring;
};

struct EProp
{
    size_t id;
    std::string outDirection;
};

int main( )
{

    const int domain_edge_length = 4;
    const int patch_edge = 1;
    assert(domain_edge_length % patch_edge == 0);
    const int patches_per_edge = domain_edge_length/patch_edge; // 6/3 = 2
    const int domain_size = domain_edge_length*domain_edge_length; // 6*6
    assert(patches_per_edge > 2);

    std::array<int, domain_size> domain_indices;
    std::iota(domain_indices.begin(), domain_indices.end(),0);

    // domain decomposition into a regular grid of patches
    std::array<
        std::array<
            std::set< int >,
            patches_per_edge
        >,
        patches_per_edge
    > patch_indices;

    for(int x : domain_indices)
    {
        int i = x/domain_edge_length;
        int j = x%domain_edge_length;
        patch_indices[i/patch_edge][j/patch_edge].insert(x);
    }

    boost::array< size_t, 2 > lengths = {{ patches_per_edge, patches_per_edge }};
    boost::grid_graph< 2 > g( lengths, {{true, true}} );
    using Traits = boost::graph_traits<boost::grid_graph<2>>;

    using indexMapType = boost::property_map<boost::grid_graph<2>, boost::edge_index_t>::const_type;
    auto indexMap(get(boost::edge_index, g));

    // Create a float for every edge in the graph
    boost::vector_property_map<std::string, indexMapType> dataMap(num_edges(g), indexMap);
    std::array<std::string , 4> directions{{ "North", "South", "East", "West"}};

    for( decltype( num_vertices(g)) i=0 ; i< num_vertices(g) ; ++i)
    {
        auto v = vertex(i,g);
        auto oEdges = out_edges(v,g);
        for(auto e(oEdges.first); e!=oEdges.second ; ++e)
        {
            auto fromx = e->first[0];
            auto fromy = e->first[1];
            auto tox = e->second[0];
            auto toy = e->second[1];
            std::string var = "Self";// ("+std::to_string(fromx)+","+std::to_string(fromy)+")->("+std::to_string(tox)+","+std::to_string(toy)+")";
            if(fromx == (tox+1)%patches_per_edge)
            {
                var = "E";// ("+std::to_string(fromx)+","+std::to_string(fromy)+")->("+std::to_string(tox)+","+std::to_string(toy)+")";
            }
            if((fromx+1)%patches_per_edge == tox)
            {
                var = "W";// ("+std::to_string(fromx)+","+std::to_string(fromy)+")->("+std::to_string(tox)+","+std::to_string(toy)+")";
            }
            if((fromy+1)%patches_per_edge == toy)
            {
                var = "S";// ("+std::to_string(fromx)+","+std::to_string(fromy)+")->("+std::to_string(tox)+","+std::to_string(toy)+")";
            }
            if(fromy == (toy+1)%patches_per_edge)
            {
                var = "N";// ("+std::to_string(fromx)+","+std::to_string(fromy)+")->("+std::to_string(tox)+","+std::to_string(toy)+")";
            }
            put(dataMap, *e, var);
        }
    }

//    for( decltype( num_vertices(g)) i=0 ; i< num_vertices(g) ; ++i)
//    {
//        auto v = vertex(i,g);
//        auto oEdges = in_edges(v,g);
//        std::cout << "Vertex " << i << std::endl;
//        for(auto e(oEdges.first); e!=oEdges.second ; ++e)
//        {
//            std::cout << get(dataMap, *e) << "  ";
//        }
//        std::cout << std::endl;
//    }

    using ComponentGridGraph = dodo::graph::SimpleBGL<VProp,EProp>;
    ComponentGridGraph cgraph;
    int total = 0;

    // add all vertices to graph
    // also add self-edges already
    std::map<boost::array<size_t,2>, int> vertexMap;
    for( decltype( num_vertices(g)) i=0 ; i< num_vertices(g) ; ++i)
    {
        auto v = vertex(i,g);
        VProp p;
        p.id = i;
        p.x = v[0];
        p.y = v[1];
        p.coordinatestring = "("+std::to_string(p.x)+ "," + std::to_string(p.y)+")";
        auto newV = cgraph.addVertex(p);
        EProp ep;
        ep.outDirection = "Self";
        cgraph.addEdge(newV, newV, ep);
        vertexMap[v] = i;
    }


    // add edges from gridgraph to cgraph
    auto allEdges = edges(g);
    for(auto e(allEdges.first); e!=allEdges.second ; ++e)
    {
        auto from = vertexMap[e->first];
        auto to = vertexMap[e->second];
        EProp ep;
        ep.outDirection = get(dataMap, *e);
        cgraph.addEdge(from, to, ep);
    }

    // set workload at position
    for(ComponentGridGraph::VertexID v : boost::make_iterator_range(cgraph.getVertices()))
    {
        VProp p = cgraph.getVertexProperty( v );
        p.coefficient =
            ( domain_edge_length / 2 - abs( p.x - domain_edge_length / 2 ) ) +
            ( domain_edge_length / 2 - abs( p.y - domain_edge_length / 2 ) );
        std::cout << p.coefficient << std::endl;
        total += p.coefficient;
        cgraph.setVertexProperty(v,p);
    }

    // set workload at position
    for(ComponentGridGraph::VertexID v : boost::make_iterator_range(cgraph.getVertices()))
    {
        VProp p = cgraph.getVertexProperty( v );
        for(ComponentGridGraph::EdgeID oei : boost::make_iterator_range(cgraph.getOutEdges(v)))
        {
            VProp neighbor = cgraph.getVertexProperty(oei.m_target);
            p.workload += neighbor.coefficient;
        }

        p.workload += 2*p.coefficient;
        p.workload *= 5;
        std::cout << p.workload << std::endl;
        cgraph.setVertexProperty(v,p);
    }

    std::ofstream ofs;
    ofs.open("/tmp/coordinate_grid.graphml");
    boost::dynamic_properties dp1;
    dp1.property("direction", boost::get(&EProp::outDirection, *cgraph.graph));
    dp1.property("coefficient", boost::get(&VProp::coefficient, *cgraph.graph));
    dp1.property("coordinates" , boost::get(&VProp::coordinatestring, *cgraph.graph));
    dp1.property("workload" , boost::get(&VProp::workload, *cgraph.graph));
    dp1.property("x" , boost::get(&VProp::x, *cgraph.graph));
    dp1.property("y" , boost::get(&VProp::y, *cgraph.graph));
    write_graphml(ofs, *cgraph.graph, dp1);
    ofs.close();
    std::cerr << total << std::endl;





    return 0;
}
