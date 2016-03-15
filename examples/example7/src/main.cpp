
#include <dodo.hpp>
#include "TestSourceMeta.hpp"
#include "TestProcessingMeta.hpp"
#include "TestSinkMeta.hpp"

template <typename T>
void printAndWriteToFile(T& graph)
{
    std::cout << graph << std::endl;
    std::ofstream file;
    file.open("/tmp/example7.graphml");
    file << graph << std::endl;
    file.close();
}

int main()
{
    using namespace dodo::components;
    meta::Network meta;

    // Create Components
    auto source1 = meta.addComponent<TestSourceMeta>();
    auto source2 = meta.addComponent<TestSourceMeta>();
    auto processing = meta.addComponent<TestProcessingMeta>();
    auto sink = meta.addComponent<TestSinkMeta>();

    // Build dependency network
    meta.addDependency(source1["out"], processing["in1"]);
    meta.addDependency(source2["out"], processing["in2"]);
    meta.addDependency(processing["out"], sink["in"]);

    auto dependencyGraph = meta.extractDependencyGraph();
    printAndWriteToFile(dependencyGraph);

    // Do the mapping based on Metadata+Hardware-graph alone.
    // Do not look at the actual implementation of the components.
    // They might be templates that are not instantiated before the
    // mapping is done!

    // Find out where you are with respect to HardwareGraph

    // Get the correct metadata-nodes from the mapping

    // Do the graybat-magic necessary to get N edges (only a single
    // cage-type for now...)

    // Find out what actual components are necessary to fit HW-Graph
    // based on found metadata-nodes

    // Instantiate all the right components as threads, collect them somewhere

    return 0;
}
