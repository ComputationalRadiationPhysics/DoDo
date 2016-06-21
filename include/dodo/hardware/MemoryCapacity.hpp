#pragma once


#include <cstddef>


namespace dodo {
    namespace hardware {

        class MemoryCapacity
        {
            std::size_t capacityKBytes;
            std::size_t filledKBytes;

        public:

            MemoryCapacity( std::size_t capacityInKBytes ) :
                capacityKBytes { capacityInKBytes },
                filledKBytes { 0 }
            { }

            auto
            getCapacity( )
            {
                return capacityKBytes;
            }

            auto
            getUsedCapacity( )
            {
                return filledKBytes;
            }

            auto
            getFreeCapacity( )
            {
                return capacityKBytes - filledKBytes;
            }

            void
            addUsedMemory( std::size_t size )
            {
                filledKBytes += size;
            }

            void
            freeMemory( )
            {
                filledKBytes = 0u;
            }

            void
            removeUsedMemory( std::size_t size )
            {
                filledKBytes -= size;
            }

        };

    }
}

