
#include <dodo.hpp>
#include "TestSourceMeta.hpp"
#include "TestProcessingMeta.hpp"
#include "TestSinkMeta.hpp"


int main()
{
    using namespace dodo::components;
    MetadataNetwork meta;

    auto source = meta.addComponent<TestSourceMeta>();
    auto processing = meta.addComponent<TestProcessingMeta>();
    auto sink = meta.addComponent<TestSinkMeta>();

    meta.addDependency(source["out"], processing["in"]);
    meta.addDependency(processing["out"], sink["in"]);

    return 0;
}
