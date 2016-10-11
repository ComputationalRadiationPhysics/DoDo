#pragma once

#include <dodo2.hpp>

class ComponentA : dodo::model::routine::ComponentBase
{
    virtual
    float
    effort() override
    {
        return 15.0;
    }

};

