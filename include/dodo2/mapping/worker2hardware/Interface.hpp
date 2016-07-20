#pragma once


#include <memory>
#include <algorithm>

#include <boost/bimap.hpp>
#include <dodo2/model/worker/Model.hpp>
#include <dodo2/model/hardware/HardwareAbstraction.hpp>
#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/utility/OneToNMap.hpp>
#include <dodo2/mapping/worker2hardware/checkMemoryLegality.hpp>


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
            public:
                using HardwareAbstraction = T_HardwareAbstraction;
                using WorkerID = model::worker::Model::WorkerID;
                using HardwareID = typename HardwareAbstraction::HardwareID;

            private:
                utility::OneToNMap<
                    HardwareID,
                    WorkerID
                > mapping;

                std::shared_ptr< model::worker::Model > workerModel;
            public:
                std::shared_ptr< HardwareAbstraction > hardwareModel;

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
                    mapping( mapping )
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
                    mapping( mapping )
                { }


                auto
                moveWorkerToCore(
                    const WorkerID w,
                    const HardwareID & h
                )
                -> void
                {
                    mapping.eraseMapping( w );
                    mapping.addMapping(
                        h,
                        w
                    );
                }


                auto
                getWorkersOfCore( const HardwareID & h )
                const -> std::vector< WorkerID >
                {
                    return mapping.one2n[h];
                }

                auto
                getWorkersInAddressSpace( const WorkerID & aSpace )
                const -> std::vector< WorkerID >
                {
                    auto w = workerModel->getWorkersInAddressSpace( aSpace );
                    return std::vector< WorkerID >(
                        w.first,
                        w.second
                    );

                }


                auto
                getHWOfWorker( const WorkerID w )
                const
                -> HardwareID
                {
                    return mapping.n2one.at( w );
                }

                auto
                listAddressSpaces( )
                const
                -> std::vector< WorkerID >
                {
                    const auto allAddressSpaces = workerModel->getAllAddressSpaces( );
                    return std::vector< WorkerID >(
                        allAddressSpaces.first,
                        allAddressSpaces.second
                    );
                }
                auto
                listWorkers( )
                const
                -> std::vector< WorkerID >
                {
                    const auto allWorkers = workerModel->getAllWorkers( );
                    return std::vector< WorkerID >(
                        allWorkers.first,
                        allWorkers.second
                    );
                }


                auto generateTrivialMapping(
                    unsigned workersPerCore,
                    const std::vector< std::string > & machineNames
                ) -> void
                {
                    auto allAddressSpaces = workerModel->getAllAddressSpaces( );
                    assert(
                        ( std::distance(
                            allAddressSpaces.first,
                            allAddressSpaces.second
                        ) == 0 )
                        && "can only be called while workerModel is still empty!"
                    );

                    std::set< HardwareID > machineElements;
                    for( const auto iname : boost::make_iterator_range( machineNames ) )
                    {
                        std::string name = iname;
                        for( const auto location :  hardwareModel->getHWElementsByName( name ) )
                        {
                            machineElements.insert( location );
                            // define a new address space at each possible location
                            auto newSpace = workerModel->newAddressSpace( );
                            moveWorkerToCore( newSpace, location ); //if it's stupid, but it works -> it's not stupid!
                        }
                    }

                    for( const auto & core : hardwareModel->getHWElementsByType( model::hardware::property::VertexType::COMPUTE ) )
                    {
                        typename HardwareAbstraction::HardwareID current = core;

                        while( machineElements.find( current ) == machineElements.end( ) )
                        {
                            current = hardwareModel->getParent( current );
                        }
                        for( unsigned i = 0; i < workersPerCore; ++i )
                        {
                            // Address space is in the same type-structure as workers,
                            // so we have to pick first element of vector
                            auto newWorker = workerModel->addWorker( mapping.one2n.at( current )[0] );
                            moveWorkerToCore(newWorker, core);
                        }

                    }

                }


            };

        } /* worker2hardware */
    } /* mapping */
} /* dodo */
