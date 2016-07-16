#pragma once

#include <memory>
#include <algorithm>

#include <boost/bimap.hpp>
#include <dodo2/model/worker/Model.hpp>
#include <dodo2/model/hardware/HardwareAbstraction.hpp>
#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/mapping/worker2hardware/Interface.hpp>
#include "Interface.hpp"
#include <dodo2/mapping/worker2hardware/Interface.hpp>


namespace dodo
{
namespace mapping
{
namespace data2worker
{

    auto
    checkMemoryBalanceAbsolute(
        const std::shared_ptr< Interface > i,
        std::vector< std::string > dataDomainNames
    )
    -> float
    {
        using DataDomain = model::data::DataDomain;
        using DataID = DataDomain::DataID;
        auto allWorkerIterators = i->workerModel->getAllWorkers();
        auto nWorkers = i->workerModel->getNumberOfWorkers();

        for( auto workerID : boost::make_iterator_range(allWorkerIterators) )
        {
            for( const std::string & name : dataDomainNames )
            {
                std::vector< Interface::DataID > d = i->getDataOfWorker( name, workerID );
                DataDomain currentDom = i->dataModel->dataDomains[name];
                for( const Interface::DataID did : d)
                {
                    float weight = currentDom.propertyManager.get("weight", did);
                }
            }


        }


        return 1;
    }


    template< typename T_HW_Abs >
    auto
    checkMemoryBalanceRelativeToCapacity(
        const std::shared_ptr< worker2hardware::Interface< T_HW_Abs > > & m,
        const std::shared_ptr< Interface > i,
        std::vector< std::string > dataDomains
    ) -> float
    {
        return 1;
    }


    template< typename T_HW_Abs >
    auto
    checkMemoryBalanceRelativeToGFLOPS(
        const std::shared_ptr< worker2hardware::Interface< T_HW_Abs > > & m,
        const std::shared_ptr< Interface > i,
        std::vector< std::string > dataDomains
    ) -> float
    {
        return 1;
    }
}

} /* data2worker */
} /* mapping */
} /* dodo */
