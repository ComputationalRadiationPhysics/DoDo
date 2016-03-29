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

template<typename T_HWElement>
struct Name
{
    using value_type = std::string;
    value_type value = "";
};


template<typename T>
struct HWVertexTemplate
{
    HWVertexTemplate(T){}
};


template<typename T_VertexData>
struct CPUCoreMeta : public HWVertexTemplate<T_VertexData>
{
    CPUCoreMeta(T_VertexData& vd) :
        HWVertexTemplate<T_VertexData>(vd)
    {

    }
};





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

struct CPUVertex : public HWVertex_t
{
    CPUVertex(utility::TreeID i, ICG a) :
        HardwareGraphVertex(i, a)
    {
        setProperty<pattr::EnergyLevel>({20});
        setProperty<pattr::Tag>({pattr::Tag::Tags::Compute});
        setProperty<pattr::Name>({"Xeon E5-2609"});

        for(unsigned i=0 ; i<4 ; ++i)
        {
            auto core = this->createChild<XeonCoreVertex>();
            auto x = interconnectGraph.lock()->connect<1>(core, id);
            x.setProperty<pattr::Bandwidth>({static_cast<size_t>(34.1 * 1024 * 1024 * 1024)});
        }
    }
};

