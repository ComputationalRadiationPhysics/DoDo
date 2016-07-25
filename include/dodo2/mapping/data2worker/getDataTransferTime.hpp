#pragma once

#include <string>

#include <dodo2/mapping/data2worker/Interface.hpp>
#include <dodo2/mapping/worker2hardware/Interface.hpp>


namespace dodo
{
namespace mapping
{
namespace data2worker
{

    template<
        typename T_HW_Abs,
        typename T_SimDom
    >
    auto getDataTransferTime(
        Interface< T_SimDom > & data2workerMapping,
        worker2hardware::Interface< T_HW_Abs > & worker2hwMaping,
        std::string const & dataDomainName,
        typename Interface< T_SimDom >::DataID const dataID,
        typename Interface< T_SimDom >::WorkerID const toWorker
    )
    -> float
    {
        size_t dataSize = data2workerMapping.dataModel-> template getProperty<std::size_t>(
            dataDomainName,
            "sizeKB",
            dataID
        );
        auto fromWorker = data2workerMapping.getWorkerHoldingData(
            dataDomainName,
            dataID
        );
        auto fromHW = worker2hwMaping.getHWOfWorker( fromWorker );
        auto toHW = worker2hwMaping.getHWOfWorker( toWorker );

        return worker2hwMaping.hardwareModel->getTransferTime(
            fromWorker,
            toWorker,
            dataSize
        );

    }

} /* data2worker */
} /* mapping */
} /* dodo */
