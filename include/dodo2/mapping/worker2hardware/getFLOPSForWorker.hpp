#pragma once

#include <memory>
#include <utility>
#include <vector>


namespace dodo
{
namespace mapping
{
namespace worker2hardware
{

    template< typename T_Interface >
    auto getFLOPSForWorker(
        const T_Interface & i
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
        > > result;
        for( auto coreMap : i.getWorkersOfAllCores( ) )
        {
            int divisor = coreMap.second.size();
            if(divisor == 0) continue;
            size_t speed = i.hardwareModel->template getProperty< size_t >(
                "VertexSpeed",
                coreMap.first
            );
            speed /= divisor;
            for( auto worker : coreMap.second )
            {
                result.push_back(std::make_pair(worker, speed));
            }
        }
        return result;
    }

    template< typename T_Interface >
    auto getFLOPSForWorker(
        std::shared_ptr< T_Interface > ip
    )
    -> std::vector<
        std::pair<
            typename T_Interface::WorkerID,
            std::size_t
        >
    >
    {
        return getFLOPSForWorker( *ip );
    }

} /* worker2hardware */
} /* mapping */
} /* dodo */
