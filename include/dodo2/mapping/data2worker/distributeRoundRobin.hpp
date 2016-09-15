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
    static
    auto
    generateRoundRobin(
        const model::data::DataDomain & pdataModel,
        model::worker::Model & pworkerModel
    ) -> utility::NToMMap<
        model::data::DataDomain::DataID,
        model::worker::Model::WorkerID
    >
    {
        utility::NToMMap<model::data::DataDomain::DataID, model::worker::Model::WorkerID> result;
        auto allWorkers = pworkerModel.getAllWorkers();
        auto currentWorker = allWorkers.first;
        auto allData = pdataModel.getDataElements();
        for(auto data : boost::make_iterator_range(pdataModel.getDataElements()))
        {
            result.insert({data, *currentWorker});
            if(currentWorker == allWorkers.second)
                currentWorker = allWorkers.first;
            else
                ++currentWorker;
        }
        return result;
    }

} /* data2worker */
} /* mapping */
} /* dodo */
