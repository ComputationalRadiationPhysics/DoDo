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

        auto
        get( typename T_Interface::HardwareID h ) const
        -> int
        {
            return participatingWorkers.at(h);
        }
    };

    template<typename T_Interface>
    struct ParticipationsMax
    {
        ParticipationsMax( const T_Interface & )
        { }

        auto
        get( typename T_Interface::HardwareID ) const
        -> int
        {
            return 1;
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
                [&i, &parts](std::size_t isum, const auto & m)
                {
                    return isum +
                           i.hardwareModel->template getProperty< std::size_t >(
                                   "MemoryUsage",
                                   m
                           ) /
                           parts.get(m);
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
