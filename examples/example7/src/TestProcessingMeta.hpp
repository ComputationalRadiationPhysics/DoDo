#pragma once

#include <dodo.hpp>

class TestProcessingMeta :
    public dodo::components::ComponentMetadataInterface
{
public:
    TestProcessingMeta()
    {
        defineInPort("in");
        defineOutPort("out");
    }

};

