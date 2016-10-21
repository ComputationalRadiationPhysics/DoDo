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
namespace component2worker
{
    template< typename T_SimDom >
    auto
    generateRoundRobin(
        const std::shared_ptr<model::routine::Abstraction< T_SimDom > > routineModel,
        std::shared_ptr<model::worker::Model> workerModel
    )
    {
        utility::OneToNMap<model::worker::Model::WorkerID, typename model::routine::Abstraction< T_SimDom >::ComponentID > result;
        auto allWorkers = workerModel->getAllWorkers();
        auto currentWorker = allWorkers.first;
        for(auto component : boost::make_iterator_range(routineModel->getAllComponents()))
        {
            result.addMapping(*currentWorker, component);
            if(currentWorker == allWorkers.second)
                currentWorker = allWorkers.first;
            else
                ++currentWorker;
        }
        return result.one2n;
    }

} /* component2worker */
} /* mapping */
} /* dodo */
