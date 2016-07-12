#pragma once


#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>


namespace dodo
{
namespace model
{
namespace hardware
{
namespace extension
{

    class InterconnectBandwidth :
        public virtual HardwareAbstractionBase
    {
        std::map<
            InterconnectGraph::EdgeID,
            std::size_t
        > id2inBW;
        PropertyManager::MapType< decltype( id2inBW ) > inBWMap;

        std::map<
            InterconnectGraph::EdgeID,
            std::size_t
        > id2inLat;
        PropertyManager::MapType< decltype( id2inLat ) > inLatMap;

        InterconnectBandwidth( ) :
            inBWMap( id2inBW ),
            inLatMap( id2inLat )
        {
            propertyManager.registerProperty(
                "InterconnectBandwidth",
                inBWMap
            );
            propertyManager.registerProperty(
                "InterconnectLatency",
                inLatMap
            );
        }

        std::size_t
        getTransferTime
        (
            const ConsistsOfGraph::TreeID & from,
            const ConsistsOfGraph::TreeID & to,
            const std::size_t dataBytes
        )
        {
            //dummy casts to avoid warnings during prototyping
            (void) from;
            (void) to;
            (void) dataBytes;
            //TODO: Find shortest path based on latency and bandwidth!
            // assume that all transfers are legal and that
            // elements themselves don't add any addditional constraints
            // For elements that don't have latency/bw annotated, assume an
            // instant transfer

            return 0;
        }

    };

} /* extension */
} /* hardware */
} /* model */
} /* dodo */
