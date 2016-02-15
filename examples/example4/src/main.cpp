#include <iostream>

#include <graybat/Cage.hpp>
#include <graybat/communicationPolicy/BMPI.hpp>
#include <graybat/graphPolicy/BGL.hpp>

#include <dout/dout.hpp>

#include <dodo.hpp>

#include "components.hpp"

int main( )
{
    decltype(auto) dout = dout::Dout::getInstance();
    dout.setVerbosity(dout::Flags::WARN | dout::Flags::INFO | dout::Flags::STAT | dout::Flags::ERROR);
    //dout.addVerbosity(dout::Flags::DEBUG);

    // dependency graph object
    auto g = std::make_shared<dodo::process::DependencyGraph>();

    // components
    auto randomV = g->create(RandomSourceMeta());
    auto splitterV = g->create(SplitterMeta());
    auto transformV = g->create(TransformMeta());
    auto prePrintV = g->create(PrintMeta());
    auto postPrintV = g->create(PrintMeta());

    // create dependencies
    // g->createDependency(randomV, RandomSourceMeta::Out0, splitterV, SplitterMeta::In0);
    // g->createDependency(splitterV, , prePrintV, 0u);

    g->createDependency(randomV.port(0), splitterV.port(0));
    g->createDependency(splitterV.port(0), prePrintV.port(0));
    g->createDependency(splitterV.port(1), transformV.port(0));
    g->createDependency(transformV.port(0), postPrintV.port(0));


    return 0;
}
