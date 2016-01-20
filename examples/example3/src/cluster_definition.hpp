#pragma once

#include <dodo.hpp>

using namespace dodo::graph;
using namespace dodo;

using ConsistsOfProperties = std::tuple<physical::attributes::Tag, physical::attributes::EnergyLevel>;
using InterconnectProperties = std::tuple<physical::attributes::Tag, physical::attributes::Bandwidth>;

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
        setProperty<physical::attributes::Tag>({physical::attributes::Tag::Tags::Switch});
    }
};


class LaserNodeVertex :
    public HWVertex_t
{
public:
    LaserNodeVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        auto cpu1 = this->createChild<CPUVertex>();
        auto cpu2 = this->createChild<CPUVertex>();
        auto fsb = this->createChild<FSBVertex>();

        auto bus1a = interconnectGraph->connect<2>(fsb, cpu1);
        //interconnectGraph->setProperty<physical::attributes::Tag>(bus1a.id, {physical::attributes::Tag::Tags::Switch});
        bus1a.setProperty<physical::attributes::Tag>( {physical::attributes::Tag::Tags::Switch} );

        auto bus1b = interconnectGraph->connect<2>(cpu1, fsb);
        auto bus2a = interconnectGraph->connect<2>(fsb, cpu2);
        auto bus2b = interconnectGraph->connect<2>(cpu2, fsb);
        auto bus3a = interconnectGraph->connect<2>(id, fsb);
        auto bus3b = interconnectGraph->connect<2>(fsb, id);

    }
};


class EthernetSwitchVertex :
    public HWVertex_t
{
public:
    EthernetSwitchVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
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
        auto cable1a = interconnectGraph->connect<2>(node1, ethSwitch);
        auto cable1b = interconnectGraph->connect<2>(ethSwitch, node2);
        auto cable2a = interconnectGraph->connect<2>(node2, ethSwitch);
        auto cable2b = interconnectGraph->connect<2>(ethSwitch, node2);
        auto cable3a = interconnectGraph->connect<2>(id, ethSwitch);
        auto cable3b = interconnectGraph->connect<2>(ethSwitch, id);

        printAllChildren();

    }
};

