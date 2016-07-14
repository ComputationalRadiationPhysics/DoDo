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

    enum class BandwidthTag
    {
        Infiniband,
        Ethernet,

        _count
    };

    struct Bandwidth
    {
        virtual
        std::size_t
        operator()( std::size_t )
        {
            return 0;
        }

    };

    struct InfiniBandwidth : private Bandwidth
    {
        std::size_t
        operator()( std::size_t transferredBytes ) override
        {
            return 0;
        }

    }


}
}
}
}

