#pragma once

#include <dodo.hpp>

using namespace dodo::graph;
using namespace dodo;

using HWVertex_t = HardwareGraphVertex<
    physical::attributes::EnergyLevel,
    physical::attributes::Bandwidth
>;

using ICG = std::shared_ptr<
    InterconnectGraph<
        physical::attributes::EnergyLevel,
        physical::attributes::Bandwidth
    >
>;



class CPUVertex :
    public HWVertex_t
{
public:
    CPUVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
    }
};

class FSBVertex :
    public HWVertex_t
{
public:
    FSBVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
    }
};


class LaserNodeVertex :
    public HWVertex_t
{
public:
    LaserNodeVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {

        localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
        auto cpu1_id = this->createChild<CPUVertex>();
        auto cpu2_id = this->createChild<CPUVertex>();
        auto fsb_id = this->createChild<FSBVertex>();
        auto bus1 = interconnectGraph->connect(mapping[fsb_id], mapping[cpu1_id]);
        auto bus2 = interconnectGraph->connect(mapping[fsb_id], mapping[cpu2_id]);
        auto bus3 = interconnectGraph->connect(mapping[id], mapping[fsb_id]);

    }
};


class EthernetSwitchVertex :
    public HWVertex_t
{
public:
    EthernetSwitchVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
    }
};


class HypnosClusterVertex :
    public HWVertex_t
{
public:
    HypnosClusterVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {

        localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
        auto node1_id = this->createChild<LaserNodeVertex>();
        auto node2_id = this->createChild<LaserNodeVertex>();
        auto switch_id = this->createChild<EthernetSwitchVertex>();
        auto cable1 = interconnectGraph->connect(mapping[node1_id], mapping[switch_id]);
        auto cable2 = interconnectGraph->connect(mapping[node2_id], mapping[switch_id]);
        auto cable3 = interconnectGraph->connect(mapping[id], mapping[switch_id]);

        printAllChildren();

    }
};

