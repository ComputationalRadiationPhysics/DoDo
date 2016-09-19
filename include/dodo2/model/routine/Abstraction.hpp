#pragma once

#include <vector>

#include <dodo2/graph/CoordinateGraph.hpp>
#include <dodo2/graph/ComponentGraph.hpp>
#include <dodo2/model/data/Abstraction.hpp>
#include "InData.hpp"
#include "EffortFunction.hpp"
#include "ComponentTemplate.hpp"


namespace dodo{
namespace model{
namespace routine{

    class ComponentBase
    {
    protected:
        using PosID = data::SimulationDomain::Graph::VertexID;
        PosID id;
        std::vector< InData > inData;
        std::vector< InData > outData;

    public:
        virtual
        float
        effort() = 0;
    };



    template<typename T_SimDom>
    class Abstraction
    {
        std::shared_ptr< data::Abstraction< T_SimDom> > dataAbstraction;
        graph::ComponentGraph g;
        std::map<
            graph::ComponentGraph::VertexID,
            std::vector< InData >
        > internal_inDataMap;
        utility::PropertyManager::MapType < decltype( internal_inDataMap ) > inDataMap;
        std::map<
            graph::ComponentGraph::VertexID,
            std::vector< InData >
        > internal_outDataMap;
        utility::PropertyManager::MapType < decltype( internal_outDataMap ) > outDataMap;
        std::map<
            graph::ComponentGraph::VertexID,
            std::string
        > internal_componentTypeMap;
        utility::PropertyManager::MapType < decltype( internal_componentTypeMap ) > componentTypeMap;
        std::map<
            graph::ComponentGraph::VertexID,
            std::string
        > internal_instancePositionMap;
        utility::PropertyManager::MapType < decltype( internal_instancePositionMap ) > instancePositionMap;

        std::map<
            std::string,
            EffortFunction
        > effortFunctions;
        utility::PropertyManager propertyManager;


    public:
        Abstraction(
            std::shared_ptr<data::Abstraction< T_SimDom > > dataAbs
        ) :
            dataAbstraction{ dataAbs },
            g{ },
            internal_inDataMap{ },
            inDataMap{ internal_inDataMap },
            internal_outDataMap{ },
            outDataMap{ internal_outDataMap },
            internal_componentTypeMap{ },
            componentTypeMap{ internal_componentTypeMap },
            internal_instancePositionMap{ },
            instancePositionMap{ internal_instancePositionMap },
            effortFunctions{ },
            propertyManager{ }
        {
            propertyManager.registerProperty("inData", inDataMap);
            propertyManager.registerProperty("outData", outDataMap);
            propertyManager.registerProperty("componentType", componentTypeMap);
            propertyManager.registerProperty("instancePosition", instancePositionMap);
        }

        void
        instantiateComponent(
            const ComponentTemplate comp
        )
        {
            for( auto c : boost::make_iterator_range( dataAbstraction->simDom->getCells( ) ) )
            {
                auto v = g.addVertex( );
                internal_instancePositionMap[v] = c;
                internal_componentTypeMap[v] = comp.name;
                internal_inDataMap.emplace( { v, std::vector<InData> ( comp.inPorts.size( ) ) } );
                std::vector<InData> & inElements = internal_inDataMap.at(v);
                internal_outDataMap.emplace( { v, std::vector<InData> ( comp.outPorts.size( ) ) } );
                std::vector<InData> & outElements = internal_outDataMap.at(v);
                int i=0;
                for( auto & p : comp.inPorts )
                {
                    data::DataDomain::DataID d = dataAbstraction->getNeighborData( p.domain, c, p.direction );
                    inElements[i] = { d, p.domain };
                    ++i;
                }
                i=0;
                for( auto & p : comp.outPorts )
                {
                    data::DataDomain::DataID d = dataAbstraction->getNeighborData( p.domain, c, p.direction );
                    outElements[i] = { d, p.domain };
                    ++i;
                }
            }
        }

    };


} /* routine */
} /* model */
} /* dodo */


