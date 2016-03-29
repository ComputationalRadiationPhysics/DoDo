#pragma once

#include <dodo.hpp>

using namespace dodo::graph;
using namespace dodo::hardware;
namespace pattr = dodo::hardware::attributes;
namespace utility = dodo::utility;

using ConsistsOfProperties = std::tuple<pattr::Tag, pattr::EnergyLevel, pattr::Name, pattr::Memorysize, pattr::Speed>;
using InterconnectProperties = std::tuple<pattr::Tag, pattr::Bandwidth, pattr::Name>;

using HWVertex_t = HardwareGraphVertex<
    ConsistsOfProperties,
    InterconnectProperties
>;

using ICG = std::weak_ptr< InterconnectGraph< InterconnectProperties > >;


struct XeonCoreVertex : public HWVertex_t
{
    XeonCoreVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        setProperty<pattr::Tag>({pattr::Tag::Tags::Compute});
        setProperty<pattr::EnergyLevel>({40});
        setProperty<pattr::Name>({"Core"});
    }


};

struct L3CacheVertex : public HWVertex_t
{
    L3CacheVertex(utility::TreeID i, ICG a, size_t size = 4*1024) :
        HardwareGraphVertex(i, a)
    {
        setProperty<pattr::Memorysize>({size});
        setProperty<pattr::Tag>({pattr::Tag::Tags::Memory});
        setProperty<pattr::Name>({"L3 Cache"});
    }
};

struct CPUVertex : public HWVertex_t
{
    CPUVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        setProperty<pattr::EnergyLevel>({20});
        setProperty<pattr::Tag>({pattr::Tag::Tags::Compute});
        setProperty<pattr::Name>({"Xeon E5-2609"});

        auto cache = this->createChild<L3CacheVertex>(static_cast<size_t>(10 * 1024));
        auto x = interconnectGraph.lock()->connect<1>(cache, id);
        x.setProperty<pattr::Bandwidth>({static_cast<size_t>(34.1 * 1024 * 1024 * 1024)});

        for(unsigned i=0 ; i<4 ; ++i)
        {
            auto core = this->createChild<XeonCoreVertex>();
            auto x = interconnectGraph.lock()->connect<1>(core, id);
            x.setProperty<pattr::Bandwidth>({static_cast<size_t>(34.1 * 1024 * 1024 * 1024)});
        }
    }
};

struct RAMVertex : public HWVertex_t
{
    RAMVertex(utility::TreeID i, ICG a, size_t size = 4096*1024) :
        HardwareGraphVertex(i, a)
    {
        setProperty<pattr::Memorysize>({size});
        setProperty<pattr::Tag>({pattr::Tag::Tags::Memory});
        setProperty<pattr::Name>({"DDR3 RAM"});
    }
};


struct FSBVertex : public HWVertex_t
{
    FSBVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        setProperty<pattr::Tag>({pattr::Tag::Tags::Switch});
        setProperty<pattr::Name>({"FSB"});
    }
};


struct LaserNodeVertex : public HWVertex_t
{
    LaserNodeVertex(utility::TreeID i, ICG a, unsigned number=0) :
        HardwareGraphVertex(i, a)
    {
        setProperty<pattr::Name>({"KeplerNode" + std::to_string(number)});
        setProperty<pattr::Tag>({pattr::Tag::Tags::Structural});

        // consistsOf elements
        auto fsb = this->createChild<FSBVertex>();
        auto bus3 = interconnectGraph.lock()->connect<1>(id, fsb);
        bus3.setProperty<pattr::Bandwidth>( {4000} );

        for(unsigned i=0; i<2 ; ++i)
        {
            auto cpu1 = this->createChild<CPUVertex>();
            auto bus1 = interconnectGraph.lock()->connect<1>(fsb, cpu1);
            bus1.setProperty<pattr::Bandwidth>( {8500} );
        }

        auto ram = this->createChild<RAMVertex>(static_cast<size_t>(256u*1024u*1024u*1024u));
        auto bus = interconnectGraph.lock()->connect<1>(fsb, ram);
        bus.setProperty<pattr::Bandwidth>({static_cast<size_t>(20*1024*1024*1024)});
    }
};


struct EthernetSwitchVertex : public HWVertex_t
{
    EthernetSwitchVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        setProperty<pattr::Tag>({pattr::Tag::Tags::Switch});
        setProperty<pattr::Name>({"Gigabit EthernetSwitch"});
    }
};


struct HypnosClusterVertex : public HWVertex_t
{
    HypnosClusterVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {

        setProperty<pattr::Name>({"Cluster"});
        setProperty<pattr::Tag>({pattr::Tag::Tags::Structural});
        setProperty<pattr::Speed>({2000});

        auto ethSwitch = this->createChild<EthernetSwitchVertex>();

        // consistsOf elements
        for(unsigned i=1 ; i<=2 ; ++i)
        {
            auto node1 = this->createChild<LaserNodeVertex>(i);

            // connection between the elements
            auto cable1 = interconnectGraph.lock()->connect<1>(node1, ethSwitch);
            cable1.setProperty<pattr::Bandwidth>( {1000} );
        }



        printAllChildren();

    }
};



