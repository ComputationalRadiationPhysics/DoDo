#pragma once

#include <dodo.hpp>

using namespace dodo::graph;
using namespace dodo;

using ConsistsOfProperties = std::tuple<physical::attributes::EnergyLevel>;
using InterconnectProperties = std::tuple<physical::attributes::Bandwidth>;

using HWVertex_t = HardwareGraphVertex<
    ConsistsOfProperties,
    InterconnectProperties
>;

using ICG = std::shared_ptr< InterconnectGraph< InterconnectProperties > >;



class CPUVertex :
    public HWVertex_t
{
public:
    CPUVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        setProperty<physical::attributes::EnergyLevel>({20});
        auto& p = getProperty<physical::attributes::EnergyLevel>();
        getProperty<physical::attributes::EnergyLevel>() = {22};
    }
};

class FSBVertex :
    public HWVertex_t
{
public:
    FSBVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        // localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
    }
};


class LaserNodeVertex :
    public HWVertex_t
{
public:
    LaserNodeVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {

        // localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
        auto cpu1 = this->createChild<CPUVertex>();
        auto cpu2 = this->createChild<CPUVertex>();
        auto fsb = this->createChild<FSBVertex>();
        auto bus1 = interconnectGraph->connect(fsb, cpu1);
        auto bus2 = interconnectGraph->connect(fsb, cpu2);
        auto bus3 = interconnectGraph->connect(id, fsb);

    }
};


class EthernetSwitchVertex :
    public HWVertex_t
{
public:
    EthernetSwitchVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        // localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
    }
};


class HypnosClusterVertex :
    public HWVertex_t
{
public:
    HypnosClusterVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {

        // localProperty.setEntry(physical::attributes::EnergyLevel({ 20 }));
        auto node1 = this->createChild<LaserNodeVertex>();
        auto node2 = this->createChild<LaserNodeVertex>();
        auto ethSwitch = this->createChild<EthernetSwitchVertex>();
        auto cable1 = interconnectGraph->connect(node1, ethSwitch);
        auto cable2 = interconnectGraph->connect(node2, ethSwitch);
        auto cable3 = interconnectGraph->connect(id, ethSwitch);

        printAllChildren();

    }
};

