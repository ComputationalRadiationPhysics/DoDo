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
        -> void
        {
            mapping.eraseMapping(w);
            mapping.addMapping(h, w);
        }

        auto
        getWorkersOfCore( const HardwareID& h )
        -> std::vector< WorkerID >
        {
            return mapping.one2n[h];
        }

        auto
        getHWOfWorker( const WorkerID w )
        -> HardwareID
        {
            return mapping.n2one.at(w);
        }

        auto
        generateTrivialMapping(
            unsigned workersPerCore,
            const std::vector< std::string > & machineNames
        )
        -> void
        {
            auto allAddressSpaces = workerModel->getAllAddressSpaces();
            assert( (std::distance(allAddressSpaces.first, allAddressSpaces.second) == 0)
                && "can only be called while workerModel is still empty!");

            for(const auto & name : boost::make_iterator_range( machineNames ) )
            {
                for( const auto & location : boost::make_iterator_range( hardwareModel->getHWElementsByName( name ) ) )
                {
                    // define a new address space at each possible location
                    auto newSpace = workerModel->newAddressSpace();
                    moveWorkerToCore(newSpace, location); //if it's stupid, but it works -> it's not stupid!
                }
            }

            for( auto & core : boost::make_iterator_range (hardwareModel->getHWElementsByType(model::hardware::property::VertexType::COMPUTE)))
            {
                typename HardwareAbstraction::HardwareID current = core;

                while( mapping.one2n.find(current) == mapping.one2n.end() )
                {
                    current = hardwareModel->getParent(current);
                }
                for(unsigned i=0; i<workersPerCore; ++i)
                {
                    // Address space is in the same type-structure as workers,
                    // so we have to pick first element of vector
                    auto newWorker = workerModel->addWorker(mapping.one2n.at(current)[0]);
                    moveWorkerToCore(newWorker, core);
                }
            };

            //TODO: needs debugging!!



        }

    };

} /* worker2hardware */
} /* mapping */
} /* dodo */
