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


struct CPUVertex : public HWVertex_t
{
    CPUVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        setProperty<physical::attributes::EnergyLevel>({20});
        setProperty<physical::attributes::Tag>({physical::attributes::Tag::Tags::Compute});
    }
};

struct FSBVertex : public HWVertex_t
{
    FSBVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        setProperty<physical::attributes::Tag>({physical::attributes::Tag::Tags::Switch});
    }
};


struct LaserNodeVertex : public HWVertex_t
{
    LaserNodeVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        // consistsOf elements
        auto cpu1 = this->createChild<CPUVertex>();
        auto cpu2 = this->createChild<CPUVertex>();
        auto fsb = this->createChild<FSBVertex>();

        // connection between the elements
        auto bus1 = interconnectGraph->connect<2>(fsb, cpu1);
        auto bus2 = interconnectGraph->connect<2>(fsb, cpu2);
        auto bus3 = interconnectGraph->connect<2>(id, fsb);

        bus1.setProperty<physical::attributes::Bandwidth>( {8500} );
        bus2.setProperty<physical::attributes::Bandwidth>( {8500} );
        bus3.setProperty<physical::attributes::Bandwidth>( {4000} );
        bus3.getProperty<physical::attributes::Bandwidth>();
    }
};


struct EthernetSwitchVertex : public HWVertex_t
{
    EthernetSwitchVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        setProperty<physical::attributes::Tag>({physical::attributes::Tag::Tags::Switch});
    }
};


struct HypnosClusterVertex : public HWVertex_t
{
    HypnosClusterVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {

        // consistsOf elements
        auto node1 = this->createChild<LaserNodeVertex>();
        auto node2 = this->createChild<LaserNodeVertex>();
        auto ethSwitch = this->createChild<EthernetSwitchVertex>();

        // connection between the elements
        auto cable1 = interconnectGraph->connect<2>(node1, ethSwitch);
        auto cable2 = interconnectGraph->connect<2>(node2, ethSwitch);

        cable1.setProperty<physical::attributes::Bandwidth>( {1000} );
        cable2.setProperty<physical::attributes::Bandwidth>( {1000} );

        printAllChildren();

    }
};

