#pragma once

#include <memory>
#include <algorithm>

#include <boost/bimap.hpp>
#include <dodo2/model/worker/Model.hpp>
#include <dodo2/model/hardware/HardwareAbstraction.hpp>
#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/utility/OneToNMap.hpp>


namespace dodo
{
namespace mapping
{
namespace worker2hardware
{


    template<
        typename T_HardwareAbstraction
    >
    class Interface
    {
        using HardwareAbstraction = T_HardwareAbstraction;

        using WorkerID = model::worker::Model::WorkerID;
        using HardwareID = typename HardwareAbstraction::HardwareID;
        std::shared_ptr< model::worker::Model > workerModel;
        std::shared_ptr< HardwareAbstraction > hardwareModel;
        utility::OneToNMap<HardwareID, WorkerID> mapping;

    public:

        Interface(
            const std::shared_ptr< model::worker::Model > & workerModel,
            const std::shared_ptr< HardwareAbstraction > & hardwareModel
        ) :
            workerModel( workerModel ),
            hardwareModel( hardwareModel )
        { }

        Interface(
            const std::shared_ptr< model::worker::Model > & workerModel,
            const std::shared_ptr< HardwareAbstraction > & hardwareModel,
            const std::map<
                WorkerID,
                HardwareID
            > & mapping
        ) :
            workerModel( workerModel ),
            hardwareModel( hardwareModel ),
            mapping(mapping)
        { }

        Interface(
            const std::shared_ptr< model::worker::Model > & workerModel,
            const std::shared_ptr< HardwareAbstraction > & hardwareModel,
            const std::map<
                HardwareID,
                std::vector< WorkerID >
            > & mapping
        ) :
            workerModel( workerModel ),
            hardwareModel( hardwareModel ),
            mapping(mapping)
        { }

        auto
        moveWorkerToCore(
            const WorkerID w,
            const HardwareID& h
        )
        {
            mapping.eraseMapping(w);
            mapping.addMapping(h, w);
        }

        auto
        getWorkersOfCore( const HardwareID& h )
        {
            return mapping.one2n[h];
        }

        auto
        getHWOfWorker( const WorkerID w )
        {
            return mapping.n2one.at(w);
        }

    };

} /* worker2hardware */
} /* mapping */
} /* dodo */
