#pragma once

#include <dodo.hpp>


class TestSourceMeta :
    public dodo::components::ComponentMetadataInterface
{
public:
    TestSourceMeta()
    {
        defineOutPort("out");
    }

};


