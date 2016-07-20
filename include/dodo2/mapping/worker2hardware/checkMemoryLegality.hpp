#pragma once

#include <memory>
#include <algorithm>

#include <boost/bimap.hpp>
#include <dodo2/model/worker/Model.hpp>
#include <dodo2/model/hardware/HardwareAbstraction.hpp>
#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/mapping/worker2hardware/Interface.hpp>


namespace dodo
{
namespace mapping
{
namespace worker2hardware
{


    template<typename T_Interface>
    bool checkMemoryLegality( const T_Interface & i )
    {
        for( auto worker : i.listWorkers() )
        {
            std::cout << "Worker: " << worker << " on HW: " << i.getHWOfWorker(worker) << std::endl;
        }
        for( auto aSpace : i.listAddressSpaces( ) )
        {
            const auto hwMemory = i.getHWOfWorker( aSpace );
            std::cout << "Address Space: " << aSpace << " on HW: " << hwMemory << std::endl;
            for( auto worker : i.getWorkersInAddressSpace( aSpace ) )
            {
                auto core = i.getHWOfWorker( worker );
                std::cout << "    Worker " << worker << " on Core " << core << std::endl;
                if( ! i.hardwareModel->isIndirectParent( hwMemory, core ) )
                {
                    std::cout << "abort " << std::endl;
                    return false;;

                }
            }
        }
        return true;
    }


    template<typename T_Interface>
    bool
    checkMemoryLegality( const std::shared_ptr<T_Interface> & i )
    {
        return checkMemoryLegality(*i);
    }

} /* worker2hardware */
} /* mapping */
} /* dodo */
