#pragma once

#include <memory>

#include <dodo2/mapping/worker2hardware/detail/collectMemoryForWorker.hpp>


namespace dodo
{
namespace mapping
{
namespace worker2hardware
{
    template< typename T_Interface >
    auto
    getMaxMemoryForWorker( const T_Interface & i )
    -> std::vector< std::pair<
            typename T_Interface::WorkerID,
            std::size_t
        >
    >
    {
        detail::ParticipationsMax<T_Interface> part(i);
        return detail::collectMemoryForWorkers(i, part, i.listWorkers());
    }

    template< typename T_Interface >
    auto
    getMaxMemoryForWorker( std::shared_ptr< T_Interface > ip )
    -> std::vector< std::pair < typename T_Interface::WorkerID, std::size_t> >

    {
        detail::ParticipationsMax<T_Interface> part(*ip);
        return detail::collectMemoryForWorkers(*ip, part, ip->listWorkers());
    }

    template< typename T_Interface >
    auto
    getFairMemoryForWorker( const T_Interface & i )
    -> std::vector< std::pair< typename T_Interface::WorkerID, std::size_t> >
    {
        detail::ParticipationsFair<T_Interface> part(i);
        return detail::collectMemoryForWorkers(i, part, i.listWorkers());
    }

    template< typename T_Interface >
    auto
    getFairMemoryForWorker( std::shared_ptr< T_Interface > ip )
    -> std::vector< std::pair< typename T_Interface::WorkerID, std::size_t> >
    {
        detail::ParticipationsFair<T_Interface> part(*ip);
        return detail::collectMemoryForWorkers(*ip, part, ip->listWorkers());
    }

} /* worker2hardware */
} /* mapping */
} /* dodo */
