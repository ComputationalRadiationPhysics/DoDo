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
        std::map<std::string, DataDomain> dataDomains;

        void addDataDomain(DataDomain&& dom)
        {
            dataDomains.insert(std::make_pair(dom.name, dom));
        }



    };

} /* data */
} /* model */
} /* dodo */
