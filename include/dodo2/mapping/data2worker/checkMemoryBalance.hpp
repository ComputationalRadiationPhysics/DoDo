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

    template< typename T_SimDom >
    auto
    checkMemoryBalanceAbsolute(
        const std::shared_ptr< Interface<T_SimDom> > data2WorkerMapping,
        std::vector< std::string > dataDomainNames
    )
    -> float
    {
        using DataDomain = model::data::DataDomain;
        using DataID = DataDomain::DataID;
        auto allWorkerIterators = data2WorkerMapping->workerModel->getAllWorkers();
        auto nWorkers = data2WorkerMapping->workerModel->getNumberOfWorkers();

        std::array<size_t, nWorkers> allWeights;
        int w=0;
        for( auto workerID : boost::make_iterator_range(allWorkerIterators) )
        {
            allWeights[w]=0;
            for( const std::string & name : dataDomainNames )
            {
                std::vector< DataID > d =
                    data2WorkerMapping->getDataOfWorker(
                        name,
                        workerID
                    );
                DataDomain currentDom = data2WorkerMapping->dataModel->dataDomains[name];
                for( DataID const did : d)
                {
                    allWeights[w] += currentDom.getProperty<std::size_t>(
                        "sizeInKB",
                        did
                    );
                }
            }
        }

        float minWeight = *std::min_element(allWeights.begin(), allWeights.end());
        float meanWeight = std::accumulate(allWeights.begin(), allWeights.end(), 0.f) / nWorkers;
        return minWeight/meanWeight;
    }


    template<
        typename T_HW_Abs,
        typename T_SimDom
    >
    auto
    checkMemoryBalanceRelativeToCapacity(
        const std::shared_ptr< worker2hardware::Interface< T_HW_Abs > > & m,
        const std::shared_ptr< Interface< T_SimDom > > i,
        std::vector< std::string > dataDomains
    ) -> float
    {
        // TODO
        return 1;
    }


    template<
        typename T_HW_Abs,
        typename T_SimDom
    >
    auto
    checkMemoryBalanceRelativeToGFLOPS(
        const std::shared_ptr< worker2hardware::Interface< T_HW_Abs > > & m,
        const std::shared_ptr< Interface< T_SimDom > > i,
        std::vector< std::string > dataDomains
    ) -> float
    {
        // TODO
        return 1;
    }

} /* data2worker */
} /* mapping */
} /* dodo */
