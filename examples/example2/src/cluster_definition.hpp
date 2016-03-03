#include <dodo.hpp>

using Energy = dodo::hardware::attributes::EnergyLevel;
using Speed = dodo::hardware::attributes::Speed;
using Memorysize = dodo::hardware::attributes::Memorysize;

struct LaserCore
    :
    dodo::graph::AttributeGraph<
        Speed,
        Energy
    >
{
    LaserCore()
    {
        auto laserCoreElement = createComputeElement( );
        laserCoreElement.setEntry( Speed{ 1200. } );
        laserCoreElement.setEntry( Energy{ 30 } );

        root = add(laserCoreElement).id;
    }
};


struct LaserCPU
    :
    dodo::graph::AttributeGraph<
        Energy,
        Speed
    >
{
    LaserCPU()
    {
        auto laserCPUElement = createStructuralElement( );
        laserCPUElement.setEntry( Energy{ 10 } );
        root = add(laserCPUElement).id;


        for(int i=0 ; i < 4 ; ++i)
            this->consistsOf(LaserCore());
    }
};

struct RAM
    :
    dodo::graph::AttributeGraph<
        Memorysize
    >
{
    RAM(size_t size)
    {
        auto ramElement = createMemoryElement( );
        ramElement.setEntry( Memorysize{ size } );

        root = add(ramElement).id;

    }
};


struct LaserNode
    :
    dodo::graph::AttributeGraph<
        Speed,
        Energy,
        Memorysize
    >
{
    LaserNode()
    {

        auto laserNodeElement = createStructuralElement( );
        laserNodeElement.setEntry( Energy{ 10 } );
        root = add(laserNodeElement).id;

        for(int i=0 ; i < 4 ; ++i)
            this->consistsOf(LaserCPU());

        for(int i=0 ; i < 4 ; ++i)
            this->consistsOf(RAM(4096));
    }
};

struct EthernetSwitch
    :
    dodo::graph::AttributeGraph<
        Energy
    >
{
    EthernetSwitch()
    {
        auto switchElement = createInterconnectElement( );
        switchElement.setEntry( Energy { 42 } );

        root = add(switchElement).id;
    }
};


struct LaserQueue
    :
    dodo::graph::AttributeGraph<
        Speed,
        Energy,
        Memorysize
    >
{

    LaserQueue()
    {
        auto x = createStructuralElement( );
        root = add(x).id;

        auto ethSwitch = this->consistsOf(EthernetSwitch());

        for(int i=0 ; i < 3 ; ++i)
        {
            auto lNode = this->consistsOf(LaserNode());
            auto connection = this->connectBidirectional(lNode, ethSwitch);

        }
    }
};

struct Hypnos
    :
    dodo::graph::AttributeGraph<
        Speed,
        Energy,
        Memorysize
    >
{

    Hypnos()
    {
        auto x = createStructuralElement( );
        root = add(x).id;

        this->consistsOf(LaserQueue());
    }
};
