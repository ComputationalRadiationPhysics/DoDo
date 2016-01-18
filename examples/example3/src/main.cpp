#include <iostream>

#include <dout/dout.hpp>

#include <dodo.hpp>

#include "cluster_definition.hpp"

struct MyPredicate{
    static bool check(HWVertex_t& current){
        bool keep = true;
        //keep |= current.getProperty<dodo::physical::attributes::EnergyLevel>().value > dodo::physical::attributes::EnergyLevel({15}).value;
        //keep |= current.getProperty<dodo::physical::attributes::Tag>().value == dodo::physical::attributes::Tag({dodo::physical::attributes::Tag::Tags::Switch}).value;
        keep = current.getProperty<dodo::physical::attributes::Tag>().value != dodo::physical::attributes::Tag({dodo::physical::attributes::Tag::           Tags::Switch}).value;

        return keep;
    }
};

int main( )
{
    decltype(auto) dout = dout::Dout::getInstance();
    dout.setVerbosity(dout::Flags::WARN | dout::Flags::INFO | dout::Flags::STAT | dout::Flags::ERROR);
    dout.addVerbosity(dout::Flags::DEBUG);

    HardwareGraph<HypnosClusterVertex> hgv;

    hgv.getSubgraph<MyPredicate>();



    return 0;
}
