#pragma once


#include <memory>
#include <algorithm>

#include <boost/bimap.hpp>
#include <dodo2/model/worker/Model.hpp>
#include <dodo2/model/hardware/HardwareAbstraction.hpp>
#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/utility/OneToNMap.hpp>
#include <dodo2/mapping/worker2hardware/checkMemoryLegality.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/iterator_range.hpp>


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
                > workerMapping;
                utility::OneToNMap<
                    HardwareID,
                    WorkerID
                > addressSpaceMapping;

                std::shared_ptr< model::worker::Model > workerModel;
            public:
                std::shared_ptr< HardwareAbstraction > hardwareModel;

                Interface(
                    const std::shared_ptr< model::worker::Model > & pworkerModel,
                    const std::shared_ptr< HardwareAbstraction > & phardwareModel
                ) :
                    workerModel( pworkerModel ),
                    hardwareModel( phardwareModel )
                { }


                Interface(
                    const std::shared_ptr< model::worker::Model > & pworkerModel,
                    const std::shared_ptr< HardwareAbstraction > & phardwareModel,
                    const std::map<
                        WorkerID,
                        HardwareID
                    > & p_mapping
                ) :
                    workerModel( pworkerModel ),
                    hardwareModel( phardwareModel )
                {
                    for( const auto & i : p_mapping )
                    {
                        if( workerModel->isWorker( i.first ) )
                        {
                            workerMapping.addMapping( i );
                        }
                        else if ( workerModel->isAddressSpace( i.first ) )
                        {
                            addressSpaceMapping.addMapping( i );
                        }
                        else
                        {
                            throw std::runtime_error(
                                "Element " +
                                i.first +
                                " is neither a worker nor an address Space!"
                            );
                        }
                    }
                }


                Interface(
                    const std::shared_ptr< model::worker::Model > & pworkerModel,
                    const std::shared_ptr< HardwareAbstraction > & phardwareModel,
                    const std::map<
                        HardwareID,
                        std::vector< WorkerID >
                    > & p_mapping
                ) :
                    workerModel( pworkerModel ),
                    hardwareModel( phardwareModel )
                {
                    for( const auto & j : p_mapping )
                    {
                        for( const auto i : j.second )
                        {
                            if( workerModel->isWorker( i ) )
                            {
                                workerMapping.addMapping( j.first, i );
                            }
                            else if( workerModel->isAddressSpace( i ) )
                            {
                                addressSpaceMapping.addMapping( j.first, i );
                            }
                            else
                            {
                                throw std::runtime_error(
                                    "Element " +
                                    i +
                                    " is neither a worker nor an address Space!"
                                );

                            }
                        }
                    }

                }


                auto
                moveWorkerToCore(
                    const WorkerID w,
                    const HardwareID & h
                )
                -> void
                {
                    workerMapping.eraseMapping( w );
                    workerMapping.addMapping(
                        h,
                        w
                    );
                }

                auto
                moveAddressSpaceToHW(
                    const WorkerID w,
                    const HardwareID & h
                )
                -> void
                {
                    addressSpaceMapping.eraseMapping( w );
                    addressSpaceMapping.addMapping(
                        h,
                        w
                    );
                }


                auto
                getWorkersOfCore( const HardwareID & h )
                const -> std::vector< WorkerID >
                {
                    return workerMapping.one2n[h];
                }

                auto
                getAddressSpacesOfHW( const HardwareID & h )
                const -> std::vector< WorkerID >
                {
                    return addressSpaceMapping.one2n[h];
                }

//                struct IsCore
//                {
//                    std::shared_ptr< T_HardwareAbstraction > hardwareModel;
//
//                    IsCore( const std::shared_ptr< T_HardwareAbstraction > & hardwareModel ) :
//                        hardwareModel( hardwareModel )
//                    { }
//                    bool operator()(HardwareID h)
//                    {
//                        return hardwareModel->template getProperty<model::hardware::property::VertexType>("VertexType", h) == model::hardware::property::VertexType::COMPUTE;
//                    }
//                };
                auto
                getWorkersOfAllCores( )
                const
                {
                    // possibility 1: copy the map to contain only workers
                    // possibility 2: store workers and address spaces in different maps!
//                    boost::filter_iterator(
//                        [ & ]( auto i )
//                        {
//                            return hardwareModel->isVertexType(
//                                i,
//                                model::hardware::property::VertexType::COMPUTE
//                            );
//                        },
//                        mapping.one2n.begin( ),
//                        mapping.one2n.end( )
//                    );
//                    return mapping.one2n;
                    return workerMapping.one2n;
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

//                auto
//                listUsedCores( )
//                const
//                -> std::vector< HardwareID >
//                {
//                }

                auto
                getHWOfWorker( const WorkerID w )
                const
                -> HardwareID
                {
                    return workerMapping.n2one.at( w );
                }

                auto
                getHWOfAddressSpace( const WorkerID w )
                const
                -> HardwareID
                {
                    return addressSpaceMapping.n2one.at( w );
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
#ifndef NDEBUG
                    auto allAddressSpaces = workerModel->getAllAddressSpaces( );
#endif
                    assert(
                        ( std::distance(
                            allAddressSpaces.first,
                            allAddressSpaces.second
                        ) == 0 )
                        && "can only be called while workerModel is still empty!"
                    );

                    std::set< HardwareID > machineElements;
                    for( const auto& iname : boost::make_iterator_range( machineNames ) )
                    {
                        for( const auto location :  hardwareModel->getHWElementsByName( iname ) )
                        {
                            machineElements.insert( location );
                            // define a new address space at each possible location
                            auto newSpace = workerModel->newAddressSpace( );
                            moveAddressSpaceToHW( newSpace, location );
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
                            auto newWorker = workerModel->addWorker( addressSpaceMapping.one2n.at( current )[0] );
                            moveWorkerToCore(newWorker, core);
                        }

                    }

                }


            };

        } /* worker2hardware */
    } /* mapping */
} /* dodo */
