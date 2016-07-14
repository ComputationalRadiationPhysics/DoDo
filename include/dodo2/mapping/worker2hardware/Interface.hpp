#pragma once

#include <memory>
#include <algorithm>

#include <boost/bimap.hpp>
#include <dodo2/model/worker/Model.hpp>
#include <dodo2/model/hardware/HardwareAbstraction.hpp>
#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>


namespace dodo
{
namespace mapping
{
namespace worker2hardware
{

    /**
     * Extensions need to be default-constructible!
     * -> Policy-Based design :)
     */
    template<
        typename T_HardwareAbstraction,
    >
    class Interface
    {
        using HardwareAbstraction = T_HardwareAbstraction;

        using WorkerID = model::worker::Model::WorkerID;
        using HardwareID = HardwareAbstraction::HardwareID;
        std::shared_ptr< model::worker::Model > workerModel;
        std::shared_ptr< HardwareAbstraction > hardwareModel;
        std::map<
            HardwareID,
            std::vector< WorkerID >
        > hw2worker;
        std::map< WorkerID, HardwareID > worker2hw;

    public:

        Interface(
            const std::shared_ptr< model::worker::Model > & workerModel,
            const std::shared_ptr< HardwareAbstraction > & hardwareModel
        ) :
            workerModel( workerModel ),
            hardwareModel( hardwareModel )
        {
        }

        Interface(
            const std::shared_ptr< model::worker::Model > & workerModel,
            const std::shared_ptr< HardwareAbstraction > & hardwareModel,
            const std::map<
                WorkerID,
                HardwareID
            > & worker2hw
        ) :
            workerModel( workerModel ),
            hardwareModel( hardwareModel ),
            worker2hw( worker2hw )
        {
            for( auto & i : worker2hw )
            {
                hw2worker[i.second].push_back( i.first );
            }
        }

        Interface(
            const std::shared_ptr< model::worker::Model > & workerModel,
            const std::shared_ptr< HardwareAbstraction > & hardwareModel,
            const std::map<
                HardwareID,
                std::vector< WorkerID >
            > & hw2worker
        ) :
            workerModel( workerModel ),
            hardwareModel( hardwareModel ),
            hw2worker( hw2worker )
        {
            for( auto & i : hw2worker )
            {
                for( auto & j : i.second )
                {
                    worker2hw.insert(
                        std::make_pair(
                            j,
                            i.first
                        )
                    );
                }
            }
        }



        auto
        moveWorkerToCore(
            const WorkerID w,
            const HardwareID& h
        )
        {
            if( worker2hw.find( w ) != worker2hw.end( ) )
            {
                const auto & currentCore = worker2hw.at( w );
                const auto & currentWorkerList = hw2worker.at( currentCore );
                std::remove(
                    currentWorkerList,
                    currentWorkerList.end( ),
                    w
                );
            }

            hw2worker[h].push_back(w);
            worker2hw[w] = h;

        }

        assignWorkerToCore( const WorkerID w, const HardwareID & core )
        {
            // worker
            assert( worker2hw.find( w ) == worker2hw.end( ) )
        }

        auto
        getHWOfWorker( const WorkerID w )
        {
            return worker2hw.at( w );
        }

    };

} /* worker2hardware */
} /* mapping */
} /* dodo */
