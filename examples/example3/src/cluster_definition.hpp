#pragma once

#include <dodo.hpp>

using namespace dodo::graph;
using namespace dodo;

using HWVertex_t = HardwareGraphVertex<
    physical::attributes::EnergyLevel,
    physical::attributes::Bandwidth
>;

using AMS_t = std::shared_ptr<
    AttributeMapStore<
        physical::attributes::EnergyLevel,
        physical::attributes::Bandwidth
    >
>;



class CPUVertex :
    public HWVertex_t
{
public:
    CPUVertex(utility::TreeID i, AMS_t a) :
        HardwareGraphVertex(i, a)
    {
        localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
    }
};


class LaserNodeVertex :
    public HWVertex_t
{
public:
    LaserNodeVertex(utility::TreeID i, AMS_t a) :
        HardwareGraphVertex(i, a)
    {

        localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
        auto cpu1_id = this->createChild<CPUVertex>();
        auto cpu2_id = this->createChild<CPUVertex>();

    }
};


class EthernetSwitchVertex :
    public HWVertex_t
{
public:
    EthernetSwitchVertex(utility::TreeID i, AMS_t a) :
        HardwareGraphVertex(i, a)
    {
        localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
    }
};


class HypnosClusterVertex :
    public HWVertex_t
{
public:
    HypnosClusterVertex(utility::TreeID i, AMS_t a) :
        HardwareGraphVertex(i, a)
    {

        localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
        auto node1_id = this->createChild<LaserNodeVertex>();
        auto node2_id = this->createChild<LaserNodeVertex>();
        auto switch_id = this->createChild<EthernetSwitchVertex>();
        auto cable1 = interconnectGraph.connect(mapping[node1_id], mapping[switch_id]);
        auto cable2 = interconnectGraph.connect(mapping[node2_id], mapping[switch_id]);
        printAllChildren();

    }
};

