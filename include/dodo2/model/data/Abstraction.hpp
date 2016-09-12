#pragma once


#include <dodo2/graph/DataGraph.hpp>
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
        T_SimDom simDom;
        using DataID = DataDomain::DataID;
        using PosID = graph::CoordinateGraph::VertexID;
        using Directions = typename T_SimDom::Directions;

        std::map<std::string, DataDomain> dataDomains;

        //TODO: add a property manager for domain-wide properties here (stuff like read-only could go there)


        void addDataDomain(DataDomain&& dom)
        {
            dataDomains.insert(std::make_pair(dom.name, dom));
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
            return dataDomains.at( domainName ).propertyManager.get(
                propertyName,
                id
            );
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
