#pragma once

#include <memory>
#include <algorithm>

#include <boost/bimap.hpp>
#include <dodo/model/worker/Model.hpp>
#include <dodo/model/hardware/HardwareAbstraction.hpp>
#include <dodo/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo/mapping/worker2hardware/Interface.hpp>
#include "Interface.hpp"
#include <dodo/mapping/worker2hardware/Interface.hpp>


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
    )
    {
//        utility::NToMMap<model::data::DataDomain::DataID, model::worker::Model::WorkerID> result;
        utility::OneToNMap<model::worker::Model::WorkerID, model::data::DataDomain::DataID> result;
        auto allWorkers = pworkerModel.getAllWorkers();
        auto currentWorker = allWorkers.first;
//        auto allData = pdataModel.getDataElements();
        for(auto data : boost::make_iterator_range(pdataModel.getDataElements()))
        {
            result.addMapping(*currentWorker, data);
            if(currentWorker == allWorkers.second)
                currentWorker = allWorkers.first;
            else
                ++currentWorker;
        }
        return result.one2n;
    }

} /* data2worker */
} /* mapping */
} /* dodo */
