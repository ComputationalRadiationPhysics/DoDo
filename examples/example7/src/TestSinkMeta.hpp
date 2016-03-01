#pragma once


#include <dodo.hpp>

class TestSinkMeta :
    public dodo::components::ComponentMetadataInterface
{
public:
    TestSinkMeta()
    {
        defineInPort("in");
    }

};


