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
    bool checkMemoryLegality( const std::shared_ptr<T_Interface> & i )
    {

        for( auto aSpace : i->workerModel->getAllAddressSpaces( ) )
        {
            const auto hwMemory = i->worker2hw.at( aSpace );
            const auto & allHWchildren = i->hardwareModel->getAllChildren( hwMemory );
            for( auto worker : i->workerModel->getWorkersInAddressSpace( aSpace ) )
            {
                if( std::find(
                    allHWchildren.begin( ),
                    allHWchildren.end( ),
                    worker
                ) == allHWchildren.end( ) )
                {
                    return false;
                }
            }
        }
        return true;
    }


} /* worker2hardware */
} /* mapping */
} /* dodo */
