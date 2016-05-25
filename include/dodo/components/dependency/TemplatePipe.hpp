#pragma once
#include <utility>
#include <numeric>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/fusion/include/for_each.hpp>

#include <dodo/components/dependency/ComponentBase.hpp>
#include <dodo/components/dependency/HierarchicalComponent.hpp>
#include <dodo/components/types/PortVertexContainer.hpp>


namespace dodo
{
namespace components
{
namespace dependency
{
    template< typename... T_Components2 >
    struct TemplatePipe :
        public HierarchicalBase
    {
        TemplatePipe(
            HierarchicalBase::Subgraph & parentGraph,
            std::string name
        ) :
            HierarchicalBase(
                parentGraph,
                name,
                { "in" },
                { "out" }
            )
        {
            using namespace boost;
            using T_Components = mpl::vector< T_Components2... >;

            // Pointers in this vector do not need to be freed; this is done by
            // their constructing class
            std::vector< ComponentBase * > hs;
            constexpr int size = fusion::result_of::size< T_Components >::value;
            using Indices = mpl::range_c< unsigned, 0u, size >;
            fusion::for_each(
                Indices{ },
                [ & ]( auto i )
                {
                    using U = typename mpl::at<T_Components, decltype(i)>::type;
                    hs.push_back(
                        createSubcomponent<U>("template_component_" + std::to_string(i))
                    );
                }
            );
            if( size > 0 )
            {
                addDependency(
                    "in",
                    hs[0]->name + "." + hs[0]->inPorts.begin( )->first
                );
                for( int i = 0; i < size - 1; ++i )
                {
                    assert( hs[i]->outPorts.size() == hs[i + 1]->inPorts.size() );
                    auto hsOutPortIt = hs[i]->outPorts.begin( );
                    auto hsInPortIt = hs[i + 1]->inPorts.begin( );
                    for( unsigned j{0}; j < hs[i]->outPorts.size( ); ++j )
                    {
                        addDependency(
                            hs[i]->name + "." + hsOutPortIt->first,
                            hs[i + 1]->name + "." + hsInPortIt->first
                        );
                        ++hsInPortIt;
                        ++hsOutPortIt;
                    }
                }
                addDependency(
                    hs[size - 1]->name + "." + hs[size - 1]->outPorts.begin( )->first,
                    "out"
                );
            }
            else
            {
                addDependency(
                    "in",
                    "out"
                );
            }
        }
    };
}
}
}