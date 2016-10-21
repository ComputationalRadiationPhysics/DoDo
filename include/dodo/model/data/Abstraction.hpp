#pragma once


#include <dodo/graph/DataGraph.hpp>
#include <dodo/model/data/Traits/Directions.hpp>
#include <dodo/model/routine/InData.hpp>
#include "SimulationDomain.hpp"
#include "DataDomain.hpp"


namespace dodo
{
namespace model
{
namespace data
{

    /**
     * Needs to be specialized with a implementation that fulfills the
     * SimulationDomainMapInterface and the SimulationDomain. See WrappedGrid2D
     * for an example.
     *
     * Has exactly 1 member of that specialization. This is used for spatial
     * information. Has also an unspecified number of DataDomains, arranged by
     * name. Data domains internally map to a
     */
    template< typename T_SimDom >
    class Abstraction
    {
    public:
        using DataID = DataDomain::DataID;
        using PosID = graph::CoordinateGraph::VertexID;
        using Directions = typename traits::Directions<T_SimDom>::Values;

        T_SimDom simDom;
        std::map<std::string, DataDomain> dataDomains;

        //TODO: add a property manager for domain-wide properties here (stuff like read-only could go there)

        Abstraction(
            T_SimDom psimDom
        ) :
            simDom{psimDom},
            dataDomains{}
        {}


        void addDataDomain(DataDomain&& dom, const std::string name)
        {
            dataDomains.insert(std::make_pair(name, dom));
        }

        template<typename T>
        auto
        getProperty(
            std::string const & domainName,
            std::string const & propertyName,
            DataID id
        ) const
        -> T
        {
            return dataDomains.at( domainName ).propertyManager.template get<T>(
                propertyName,
                id
            );
        }

        template<typename T>
        auto
        getProperty(
            routine::InData const & dataElement,
            std::string const & propertyName
        ) const
        -> T
        {
            return getProperty<T>(
                dataElement.domain,
                propertyName,
                dataElement.id
            );
        }

        template<typename T>
        auto
        setProperty(
            std::string const & domainName,
            std::string const & propertyName,
            DataID id,
            T property
        )
        -> void
        {
            dataDomains.at( domainName ).setProperty(propertyName, id, property);
        }

        auto
        getNeighborData(
            std::string const & domainName,
            PosID posID,
            Directions dir
        )
        -> DataID
        {
            PosID neighborPos = simDom.getNeighbor(
                posID,
                dir
            );
            return dataDomains.at( domainName ).getDataAtPos( neighborPos );
        }

        auto
        getNeighborData(
            std::string const & domainName,
            PosID posID
        )
        -> DataID
        {
            return dataDomains.at( domainName ).getDataAtPos( posID );
        }



    };

} /* data */
} /* model */
} /* dodo */
