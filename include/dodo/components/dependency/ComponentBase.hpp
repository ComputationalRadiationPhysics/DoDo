#pragma once


#include <numeric>
#include <map>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <boost/mpl/range_c.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/subgraph.hpp>

#include <dodo/components/types/PortVertexContainer.hpp>


namespace dodo
{
    namespace components
    {
        namespace dependency
        {

            struct ComponentBase
            {
                // use this typedef when writing graphviz (dot) files
                using GraphvizAttributes = std::map<std::string, std::string>;
                using DotGraph = boost::adjacency_list<
                    boost::vecS,
                    boost::vecS,
                    boost::directedS,
                    boost::property<
                        boost::vertex_index_t,
                        size_t,
                         boost::property<
                            boost::vertex_attribute_t,
                            GraphvizAttributes,
                            types::PortVertexContainer
                        >
                    >,
                    boost::property<
                        boost::edge_index_t,
                        size_t,
                        boost::property<
                            boost::edge_attribute_t,
                            GraphvizAttributes
                        >
                    >,
                    boost::property<
                        boost::graph_name_t,
                        std::string,
                        boost::property<
                            boost::graph_graph_attribute_t,
                            GraphvizAttributes,
                            boost::property<
                                boost::graph_vertex_attribute_t,
                                GraphvizAttributes,
                                boost::property<
                                    boost::graph_edge_attribute_t,
                                    GraphvizAttributes
                                >
                            >
                        >
                    >
                >;

//                // use this typedef when writing graphml files (for yEd)
//                using XMLGraph = boost::adjacency_list<
//                    boost::vecS,
//                    boost::vecS,
//                    boost::directedS,
//                    boost::property<
//                        boost::vertex_index_t,
//                        size_t,
//                        types::PortVertexContainer
//                    >,
//                    boost::property<
//                        boost::edge_index_t,
//                        size_t
//                    >
//                >;
                using Subgraph = boost::subgraph< DotGraph >;
                Subgraph & sg;

                // fully qualified name
                std::string fullName;

                // Stored Vertex descriptors are local wrt the subgraph sg
                // port names are stored as local names
                std::map< std::string, Subgraph::vertex_descriptor> inPorts;
                std::map< std::string, Subgraph::vertex_descriptor> outPorts;
                std::string name;


                template<
                    typename T_InPorts,
                    typename T_OutPorts
                >
                ComponentBase(
                    Subgraph & parentGraph,
                    std::string fullName,
                    T_InPorts in,
                    T_OutPorts out
                ) :
                    sg { parentGraph.create_subgraph( ) },
                    fullName { fullName }
                {
                    auto pos = fullName.rfind( '.' );
                    if( pos == std::string::npos )
                    {
                        name = fullName;
                    }
                    else
                    {
                        name = fullName.substr( pos + 1 );
                    }
                    initPorts( inPorts, in, "in" );
                    initPorts( outPorts, out, "out" );
                }


                void initPorts(
                    decltype( inPorts ) & portMap,
                    const std::vector< std::string >
                    names,
                    const std::string
                )
                {
                    for( const auto & s : names )
                    {
                        portMap[s] = add_vertex( sg );
                    }
                }


                void initPorts(
                    decltype( inPorts ) & portMap,
                    int count,
                    const std::string label = ""
                )
                {
                    std::vector< std::string>
                    sv( count );
                    int c = 0;
                    std::generate(
                        sv.begin( ), sv.end( ),
                        [ & ]( )
                        {
                            return label + std::to_string( ++c );
                        }
                    );
                    initPorts( portMap, sv, "" );
                }


                auto getInPortGlobal( const std::string s )
                {
                    return sg.local_to_global( inPorts.at( s ) );
                }


                auto getOutPortGlobal( const std::string s )
                {
                    return sg.local_to_global( outPorts.at( s ) );
                }

                void addDependencyGlobal(
                    Subgraph::vertex_descriptor from,
                    Subgraph::vertex_descriptor to
                )
                {
                    add_edge(
                        from,
                        to,
                        sg.root( )
                    );
                }

                void addDependency(
                    std::string from,
                    std::string to
                )
                {
                    auto fromVertex = findVertexFromString( from );
                    auto toVertex = findVertexFromString( to );
                    addDependencyGlobal(
                        fromVertex,
                        toVertex
                    );
                }

                virtual ~ComponentBase( )
                { }

                virtual
                Subgraph::vertex_descriptor
                findVertexFromString( std::string s )
                {
                    if( inPorts.find( s ) != inPorts.end( ) )
                    {
                        return getInPortGlobal( s );
                    }
                    else if( outPorts.find( s ) != outPorts.end( ) )
                    {
                        return getOutPortGlobal( s );
                    }
                    throw std::domain_error( "Port " + s + " does not exist!" );
                }
            };

        }
    }
}


