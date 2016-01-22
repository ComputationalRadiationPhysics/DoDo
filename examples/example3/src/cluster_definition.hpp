#pragma once

#include <dodo.hpp>

using namespace dodo::graph;
namespace pattr = dodo::physical::attributes;
namespace utility = dodo::utility;

using ConsistsOfProperties = std::tuple<pattr::Tag, pattr::EnergyLevel>;
using InterconnectProperties = std::tuple<pattr::Tag, pattr::Bandwidth>;

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
        setProperty<pattr::EnergyLevel>({20});
        setProperty<pattr::Tag>({pattr::Tag::Tags::Compute});
    }
};

struct FSBVertex : public HWVertex_t
{
    FSBVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        setProperty<pattr::Tag>({pattr::Tag::Tags::Switch});
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

        bus1.setProperty<pattr::Bandwidth>( {8500} );
        bus2.setProperty<pattr::Bandwidth>( {8500} );
        bus3.setProperty<pattr::Bandwidth>( {4000} );
    }
};


struct EthernetSwitchVertex : public HWVertex_t
{
    EthernetSwitchVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        setProperty<pattr::Tag>({pattr::Tag::Tags::Switch});
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

        cable1.setProperty<pattr::Bandwidth>( {1000} );
        cable2.setProperty<pattr::Bandwidth>( {100} );

        printAllChildren();

    }
};

