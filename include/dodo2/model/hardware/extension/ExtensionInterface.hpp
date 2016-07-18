#pragma once

#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/model/hardware/property/Bandwidth.hpp>


namespace dodo
{
namespace model
{
namespace hardware
{
namespace extension
{

    struct ExtensionInterface
    {
        virtual
        void addPropertyToDPWriter(
            boost::dynamic_properties &,
            std::list< std::shared_ptr< void > > &
        )
        { }

    };

} /* extension */
} /* hardware */
} /* model */
} /* dodo */
