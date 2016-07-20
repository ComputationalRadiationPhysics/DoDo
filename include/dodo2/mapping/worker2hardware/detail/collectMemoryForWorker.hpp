#pragma once

#include <memory>
#include <vector>
#include <map>
#include <numeric>


namespace dodo
{
namespace mapping
{
namespace worker2hardware
{
namespace detail
{

    template<typename T_Interface>
    struct ParticipationsFair
    {
        std::map<
            typename T_Interface::HardwareID,
            int
        > participatingWorkers;

        ParticipationsFair( const T_Interface & i )
        {
            for(auto worker : i.listWorkers( ) )
            {
                typename T_Interface::HardwareID core = i.getHWOfWorker( worker );
                auto memories = i.hardwareModel->getBackingMemories( core );
                for( auto memory : memories )
                    participatingWorkers[memory]++;
            }
        }
    };

    template<typename T_Interface>
    struct ParticipationsMax
    {
        std::map<
            typename T_Interface::HardwareID,
            int
        > participatingWorkers;

        ParticipationsMax( const T_Interface & i )
        {
            for(auto worker : i.listWorkers( ) )
            {
                typename T_Interface::HardwareID core = i.getHWOfWorker( worker );
                auto memories = i.hardwareModel->getBackingMemories( core );
                for( auto memory : memories )
                    participatingWorkers[memory] = 1;
            }
        }
    };


    template<
        typename T_Interface,
        typename T_Participations
    >
    auto collectMemoryForWorkers(
        const T_Interface & i,
        const T_Participations & parts,
        const std::vector< typename T_Interface::WorkerID > & allWorkers
    )
    -> std::vector<
        std::pair<
            typename T_Interface::WorkerID,
            std::size_t
        >
    >
    {
        std::vector< std::pair<
                typename T_Interface::WorkerID,
                std::size_t
            >
        > result;
        for( auto worker : allWorkers )
        {
            typename T_Interface::HardwareID core = i.getHWOfWorker( worker );
            const auto memories = i.hardwareModel->getBackingMemories( core );
            std::size_t sum = std::accumulate(
                memories.begin( ),
                memories.end( ),
                0lu,
                [&i, &parts](std::size_t sum, const auto & m)
                {
                    return sum +
                           i.hardwareModel->template getProperty< std::size_t >(
                                   "MemoryUsage",
                                   m
                           ) /
                           const_cast< T_Participations & >( parts )
                               .participatingWorkers[m];
                }
            );
            result.push_back(
                std::make_pair(
                    worker,
                    sum
                )
            );
        }
        return result;
    }


} /* detail */
} /* worker2hardware */
} /* mapping */
} /* dodo */
