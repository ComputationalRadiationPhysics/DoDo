#pragma once

#include <cassert>

#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <dodo2/graph/SimpleBGL.hpp>
#include <dodo2/model/worker/property/NodeType.hpp>
#include <dodo2/model/worker/property/Bundle.hpp>


namespace dodo
{
namespace model
{
namespace worker
{

    class Model
    {
        using WorkerGraph = graph::SimpleBGL<
            property::Bundle,
            boost::no_property,
            boost::vecS,
            boost::listS
        >;

        WorkerGraph g;

        struct TypeFilter
        {
            property::NodeType t;
            WorkerGraph* gInner;

            TypeFilter( )
            { };


            TypeFilter(
                property::NodeType p,
                WorkerGraph * pgInner
            ) :
                t( p ),
                gInner( pgInner )
            { };

            bool
            operator()( WorkerGraph::VertexID id )
            {
//                std::cerr << "submitted Type: " << t << " actualType: " << gInner->getVertexProperty( id ).type << std::endl;
                return gInner->getVertexProperty( id ).type == t;
            }
        };

//        struct FindWorkerFilter
//        {
//            WorkerGraph::VertexID aSpace;
//            WorkerGraph* gInner;
//
//            FindWorkerFilter( )
//            { };
//
//
//            FindWorkerFilter(
//                WorkerGraph::VertexID p,
//                WorkerGraph * pg
//            ) :
//                aSpace( p ),
//                gInner( pg )
//            { };
//
//            bool
//            operator()( WorkerGraph::VertexID id )
//            {
//                const property::Bundle prop = gInner->getVertexProperty( id );
////                std::cerr << "submitted Type: WORKER" << " actualType: " << gInner->getVertexProperty( id ).type << std::endl;
//                bool type = prop.type == property::NodeType::WORKER;
//                bool s = gInner->getOutEdges( id ).first->m_target == aSpace;
//                return type && s;
//            }
//        };

//        struct IsWorkerFilter
//        {
//            WorkerGraph* gInner;
//            IsWorkerFilter( WorkerGraph* gParam ) :
//                gInner(gParam)
//            {}
//
//            bool operator()( WorkerGraph::VertexID id )
//            {
//                const property::Bundle prop = gInner->getVertexProperty( id );
//                return prop.type == property::NodeType::WORKER;
//            }
//        };


    public:

        bool isWorker(  WorkerGraph::VertexID id )
        {
            const property::Bundle prop = g.getVertexProperty( id );
            return prop.type == property::NodeType::WORKER;
        }

        bool isAddressSpace(  WorkerGraph::VertexID id )
        {
            const property::Bundle prop = g.getVertexProperty( id );
            return prop.type == property::NodeType::ADDRESS_SPACE;
        }

        Model() = default;

        explicit
        Model(size_t addressSpaces, size_t workersPerSpace)
        {
            for(size_t i=0 ; i< addressSpaces ; ++i)
            {
                auto aSpace = newAddressSpace();
                for(size_t j=0 ; j<workersPerSpace ; ++j)
                {
                    addWorker(aSpace);
                }
            }
        }

        using WorkerID = WorkerGraph::VertexID;

        WorkerID
        newAddressSpace( )
        {
            WorkerID id = g.addVertex( );
            property::Bundle b;
            b.type = property::NodeType::ADDRESS_SPACE;
            g.setVertexProperty(
                id,
                b
            );
            return id;
        }


        WorkerID
        addWorker( const WorkerID addressSpace )
        {
//            std::cerr << g.getVertexProperty( addressSpace ).type << std::endl;
            assert(
                g.getVertexProperty( addressSpace ).type
                == property::NodeType::ADDRESS_SPACE
            );

            WorkerID id = g.addVertex( );
            property::Bundle b;
            b.type = property::NodeType::WORKER;
            g.setVertexProperty(
                id,
                b
            );
            g.addEdge(
                id,
                addressSpace
            );
            return id;
        }

        auto
        getAllWorkers( )
        {
            TypeFilter filter(
                property::NodeType::WORKER,
                &g
            );
            boost::filtered_graph<
                WorkerGraph::BGLGraph,
                boost::keep_all,
                TypeFilter
            > fg(
                *g.graph,
                boost::keep_all(),
                filter
            );
            return boost::vertices( fg );
        }

        auto
        getNumberOfWorkers( ) const
        {
            return g.numVertices();
        }

        auto
        getAllAddressSpaces( )
        {
            TypeFilter filter(
                property::NodeType::ADDRESS_SPACE,
                &g
            );
            boost::filtered_graph<
                WorkerGraph::BGLGraph,
                boost::keep_all,
                TypeFilter
            > fg(
                *g.graph,
                boost::keep_all( ),
                filter
            );
            return boost::vertices( fg );
        }

        WorkerID
        getAddressSpaceFor( const WorkerID worker ) const
        {
            return g.getOutEdges( worker ).first->m_target;
        }


        auto
        getWorkersInAddressSpace(
            const WorkerID addressSpace
        )
        {
//            FindWorkerFilter filter(
//                addressSpace,
//                &g
//            );
//            boost::filtered_graph<
//                WorkerGraph::BGLGraph,
//                boost::keep_all,
//                FindWorkerFilter
//            > fg(
//                *g.graph,
//                boost::keep_all( ),
//                filter
//            );
////            std::cerr << "nAddressSpaces: " << boost::num_vertices(fg) << std::endl;
//            return boost::vertices( fg );
//            return g.getInEdges(addressSpace);
            return g.getAdjacentVertices(addressSpace);
        }

    };

} /* worker */
} /* model */
} /* dodo */
