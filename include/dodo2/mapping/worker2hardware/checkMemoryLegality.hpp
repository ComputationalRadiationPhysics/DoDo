#pragma once

#include <memory>


namespace dodo
{
namespace mapping
{
namespace worker2hardware
{

    template< typename T_Interface >
    auto
    checkMemoryLegality( const T_Interface & i )
    -> bool
    {
//        for( auto worker : i.listWorkers() )
//        {
//            std::cout << "Worker: " << worker <<
//                " on HW: " << i.getHWOfWorker(worker) << std::endl;
//        }
        for( auto aSpace : i.listAddressSpaces( ) )
        {
            const auto hwMemory = i.getHWOfAddressSpace( aSpace );
//            std::cout << "Address Space: " << aSpace <<
//                " on HW: " << hwMemory << std::endl;
            for( auto worker : i.getWorkersInAddressSpace( aSpace ) )
            {
                auto core = i.getHWOfWorker( worker );
//                std::cout << "    Worker " << worker <<
//                    " on Core " << core << std::endl;
                if( ! i.hardwareModel->isIndirectParent( hwMemory, core ) )
                {
                    return false;;

                }
            }
        }
        return true;
    }


    template< typename T_Interface >
    auto
    checkMemoryLegality( const std::shared_ptr<T_Interface> & i )
    -> bool
    {
        return checkMemoryLegality(*i);
    }

} /* worker2hardware */
} /* mapping */
} /* dodo */
