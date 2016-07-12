#pragma once


#include <cstddef>


namespace dodo
{
namespace model
{
namespace hardware
{
namespace property
{

    class MemoryCapacity
    {
        using MemSize_t = std::size_t;
        MemSize_t capacityKBytes;
        MemSize_t filledKBytes;

    public:

        MemoryCapacity( MemSize_t capacityInKBytes ) :
            capacityKBytes { capacityInKBytes },
            filledKBytes { 0 }
        { }


        void
        setTotalCapacity ( MemSize_t size )
        {
            capacityKBytes = size;
        }


        auto
        getTotalCapacity( ) const
        {
            return capacityKBytes;
        }


        auto
        getUsedMemory( ) const
        {
            return filledKBytes;
        }


        auto
        getFreeCapacity( ) const
        {
            return capacityKBytes - filledKBytes;
        }


        void
        addUsedMemory( MemSize_t size )
        {
            filledKBytes += size;
        }

        void
        freeMemory( )
        {
            filledKBytes = 0;
        }


        void
        reduceUsedMemory( MemSize_t size )
        {
            if( size > filledKBytes )
            {
                freeMemory( );
            }
            else
            {
                filledKBytes -= size;
            }
        }
    };

    std::ostream & operator<<(
        std::ostream & ostream1,
        const MemoryCapacity & memC
    )
    {
        return ostream1 << memC.getCapacity( );
    }

}
}
}
}

