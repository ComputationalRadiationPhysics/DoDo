#pragma once

#include <dodo.hpp>


struct LaserNode : dodo::hardware::HWNode
{
    LaserNode(ID id)
    : HWNode(id)
    {}

    template<typename T_Dispatcher>
    void accept(T_Dispatcher &d)
    {
        d.dispatch(*this);
    }

};

struct Cluster : dodo::hardware::HWNode
{
    std::vector< std::shared_ptr< LaserNode > > nodes;

    Cluster(ID id) :
        HWNode(id)
    {
        for( int i = 0; i < 2; ++i )
        {
            nodes.push_back( createChild< LaserNode >( ) );
        }
    }

    template<typename T_Dispatcher>
    void accept(T_Dispatcher &d)
    {
        d.dispatch(*this);
    }
};

using HWList = boost::fusion::vector<Cluster, LaserNode>;


