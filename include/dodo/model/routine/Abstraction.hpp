#pragma once

#include <vector>

#include <boost/container/flat_set.hpp>

#include <dodo/graph/CoordinateGraph.hpp>
#include <dodo/graph/ComponentGraph.hpp>
#include <dodo/model/data/Abstraction.hpp>
#include "InData.hpp"
#include "ComponentTemplate.hpp"
#include "ComponentBase.hpp"
#include "detail/DependencyInfo.hpp"


namespace dodo{
namespace model{
namespace routine{

    template<typename T_SimDom>
    class Abstraction
    {
    public:
        using DataAbstraction = data::Abstraction<T_SimDom>;
        using ComponentID = graph::ComponentGraph::VertexID;
    private:
        std::shared_ptr< DataAbstraction > dataAbstraction;
        graph::ComponentGraph g;
        std::map<
            graph::ComponentGraph::VertexID,
            std::shared_ptr< ComponentBase< DataAbstraction> >
        > internal_componentMap;
        utility::PropertyManager::MapType< decltype( internal_componentMap ) >
            componentMap;
        utility::PropertyManager propertyManager;
        boost::container::flat_set< std::string > instantiatedComponents;
        std::vector< detail::DependencyInfo > remainingDependencies;



        auto
        addDependenciesForPredecessors(
            const boost::container::flat_set<std::string> & predecessors,
            const graph::CoordinateGraph::VertexID position,
            const graph::ComponentGraph::VertexID successor
        )
        -> void
        {
            // for each instance , add dependencies to predecessor-vertices
            for( const std::string & pred : predecessors )
            {
                const auto & predecessor = g.inverseInstanceMap.at(
                    make_pair( pred, position )
                );
                g.addDependency( predecessor, successor );
            }
        }


        using Directions = typename data::traits::Directions< T_SimDom >::Values;

        void addDataToPorts(
            std::vector< Port< Directions > > const & ports,
            graph::CoordinateGraph::VertexID position,
            std::vector< InData > & inElements
        ) const
        {
            size_t i=0;
            inElements.resize( ports.size() );
            for( auto & p : ports )
            {
                data::DataDomain::DataID d =
                dataAbstraction->getNeighborData(
                    p.domain,
                    position,
                    p.direction
                );
                inElements[i] = { d, p.domain };
                ++i;
            }
        }


        template<
            typename T_ComponentBase,
            bool T_Deps
        >
        auto
        instantiateInternal(
            ComponentTemplate<
                T_ComponentBase
            > const & comp
        )
        -> void
        {
            // type of c: coordinateGraph vertex descriptor
            for( auto c : boost::make_iterator_range(
                dataAbstraction->simDom.getCells( )
            ) )
            {
                // create a vertex for each position in the physical domain
                auto v = g.addVertex( comp.name, c );

                // create a component that is associated with the vertex
                std::shared_ptr< ComponentBase< DataAbstraction > > component =
                    std::make_shared< T_ComponentBase >( );
                internal_componentMap[v] = component;

                // add data elements for the ports
                addDataToPorts( comp.inPorts, c, component->inData );
                addDataToPorts( comp.outPorts, c, component->outData );

                // Should dependencies be added directly or later?
                // should be eliminated at compile-time, if false
                if( T_Deps )
                {
                    addDependenciesForPredecessors(comp.predecessors , c, v);
                }
            }
        }


        auto
        addDependencies(
            const detail::DependencyInfo & info
        )
        -> void
        {
            // iterate over vertices that represent instances of the successor
            for( auto pos : boost::make_iterator_range(
                dataAbstraction->simDom.getCells( )
            ) )
            {
                const auto v = g.inverseInstanceMap.at(
                    std::make_pair( info.name, pos )
                );
                addDependenciesForPredecessors( info.predecessors, pos, v );
            }
        }

        auto
        addRemainingDependencies( )
        -> void
        {
            for( const detail::DependencyInfo & info : remainingDependencies )
            {
                addDependencies( info );
            }
        }

        template< typename T_ComponentBase >
        auto
        instantiateComponent(
            const ComponentTemplate<
                T_ComponentBase
            > comp
        )
        -> void
        {
            if( instantiatedComponents.count( comp.name ) != 0 )
            {
                std::cerr << "A component may not be instantiated twice!"
                          << std::endl;
            }

            if( !comp.predecessors.empty( ) &&
                std::includes(
                    instantiatedComponents.begin( ),
                    instantiatedComponents.end( ),
                    comp.predecessors.begin( ),
                    comp.predecessors.end( )
                )
            )
            {
                //it works, but needs dependency handling
                instantiateInternal< T_ComponentBase, true >( comp );
            }
            else
            {
                // Either no dependency exists or the
                // predecessor is required but does not exist.
                // dependency handling must be completed at the end
                // (is done by recursive call if instantiateComponents)
                instantiateInternal< T_ComponentBase, false >( comp );
                remainingDependencies.push_back( comp );
            }
            instantiatedComponents.insert( comp.name );
        }

    public:
        Abstraction(
            std::shared_ptr<data::Abstraction< T_SimDom > > dataAbs
        ) :
            dataAbstraction{ dataAbs },
            g{ },
            internal_componentMap{ },
            componentMap{ internal_componentMap },
            propertyManager{ },
            instantiatedComponents{ },
            remainingDependencies{ }
        {
            propertyManager.registerProperty("component", componentMap);
        }


//         recursive function to instantiate all component templates
//         and add the required dependencies
        template<
            typename T_ComponentTemplate,
            typename... T_Rest
        >
        auto
        instantiateComponents(
            T_ComponentTemplate temp,
            T_Rest... rest
        )
        -> void
        {
            instantiateComponent( temp );
            instantiateComponents<T_Rest...>( rest... );
        }


        template< typename... Ts >
        auto
        instantiateComponents( Ts... )
        -> typename std::enable_if< sizeof...( Ts ) == 0 >::type
        {
            addRemainingDependencies();
        }

        auto
        getAllComponents() const
        -> std::pair<
            graph::ComponentGraph::AllVertexIter,
            graph::ComponentGraph::AllVertexIter
        >
        {
            return g.getVertices();
        }

// Alternative implementation, using boost fusion (not so nice to read?)
//        template< typename... T_ComponentStencils >
//        auto
//        instantiateComponents(
//            T_ComponentStencils... stencils
//        )
//        -> void
//        {
//            boost::fusion::for_each(
//                boost::fusion::vector< T_ComponentStencils... >(stencils...),
//                [this](auto & i)
//                {
//                    instantiateComponent( i );
//                }
//            );
//            addRemainingDependencies();
//        }

    };


} /* routine */
} /* model */
} /* dodo */


