#include <iostream>

#include <dout/dout.hpp>

#include <dodo.hpp>
#include "TestProcessing.hpp"
#include "TestSink.hpp"
#include "TestSource.hpp"


int main( )
{
    using namespace dodo::components;
    MetadataNetworkImpl net;

    auto source = net.addComponent<TestSource>();
    auto processing = net.addComponent<TestProcessing>();
    auto sink = net.addComponent<TestSink>();

    net.addDependency(source["A"], processing["in"]);
    net.addDependency(processing["out"], sink["in"]);

    net.enable();

    return 0;
}
