#pragma once


#include <string>
#include <sstream>
#include "dodo/types/Base.hpp"


namespace dodo
{
    namespace components
    {
        namespace types
        {

            struct ResourceID :
                public dodo::types::Base<std::string>
            {
                ResourceID() = default;


                ResourceID(Base::value_type s) :
                    Base(s)
                {};


                friend std::string toString(const ResourceID &id)
                {
                    std::ostringstream oss;
                    oss << "ResourceID: " << id;
                    return oss.str();
                }
            };

        }
    }
}
