#include <iostream>

#include <dout/dout.hpp>

#include <dodo.hpp>

#include "cluster_definition.hpp"

int main( )
{
    decltype(auto) dout = dout::Dout::getInstance();
    dout.setVerbosity(dout::Flags::WARN | dout::Flags::INFO | dout::Flags::STAT | dout::Flags::ERROR);
    dout.addVerbosity(dout::Flags::DEBUG);

    HardwareGraph<HypnosClusterVertex> hgv;


    auto subgraph = hgv.getSubgraph(
        [](auto& i)
        {
            using namespace dodo::physical::attributes;
            return i.template getProperty<Tag>().value != Tag({Tag::Tags::Switch}).value;
        }
    );



    return 0;
}
