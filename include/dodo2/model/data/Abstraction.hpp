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
        std::map<std::string, DataDomain> dataDomains;

        void addDataDomain(std::string name, DataDomain&& dom)
        {
            dataDomains.insert(std::make_pair(name, dom));
        }



    };

} /* data */
} /* model */
} /* dodo */
