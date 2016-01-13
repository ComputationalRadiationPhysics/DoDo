#include <iostream>

#include <dout/dout.hpp>

#include <dodo.hpp>

#include "cluster_definition.hpp"


int main( )
{
    decltype(auto) dout = dout::Dout::getInstance();
    dout.setVerbosity(dout::Flags::WARN | dout::Flags::INFO | dout::Flags::STAT | dout::Flags::ERROR);
    //dout.addVerbosity(dout::Flags::DEBUG);

    auto interconnectGraph = std::make_shared<dodo::graph::InterconnectGraph<InterconnectProperties>>();


    HypnosClusterVertex hgv(dodo::utility::TreeID(), interconnectGraph);

    return 0;
}
