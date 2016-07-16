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
