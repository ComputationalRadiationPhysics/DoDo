#include <iostream>

#include <dout/dout.hpp>

#include <dodo.hpp>

#include "cluster_definition.hpp"


int main( )
{
    decltype(auto) dout = dout::Dout::getInstance();
    dout.setVerbosity(dout::Flags::WARN | dout::Flags::INFO | dout::Flags::STAT | dout::Flags::ERROR);
    //dout.addVerbosity(dout::Flags::DEBUG);

    auto ams = std::make_shared<dodo::graph::AttributeMapStore<dodo::physical::attributes::EnergyLevel, dodo::physical::attributes::Bandwidth>>();

    HypnosClusterVertex hgv(dodo::utility::TreeID(), ams);

    return 0;
}
