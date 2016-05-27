#pragma once

#include <boost/bimap.hpp>
#include <boost/bimap/unordered_multiset_of.hpp>


namespace dodo
{
namespace mapping
{

    class SimpleMapping
    {

        using ComponentGraph = boost::adjacency_list<>;
        using HWGraph = boost::adjacency_list<>;
        // probably one patchGraph for each domain!
        using PatchGraph = boost::adjacency_list<>;

        // TODO: also create a mapping for the edges
        // -> no 1:1 mapping
        // HW-Edge  might accommodate multiple dependency-connections
        // dependency can only be mapped to 1 HW-Edge (some HW-edges might be unused!)
        using HWMapping = boost::bimap<
            boost::bimaps::unordered_multiset_of< HWGraph::vertex_descriptor >,
            boost::bimaps::set_of < ComponentGraph::vertex_descriptor >
        >;

        // Map only to ports!
        // one patch might be mapped to multiple ports
        // each port can have at most 1 patch
        // patches from different domains need their own port!
        // TODO: use a filtered Graph that selects ports
        using PatchMapping = boost::bimap<
            boost::bimaps::unordered_multiset_of< ComponentGraph::vertex_descriptor >,
            boost::bimaps::set_of< PatchGraph::vertex_descriptor >
        >;


    };


}
}


