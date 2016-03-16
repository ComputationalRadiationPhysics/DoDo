#pragma once

#include <sstream>
#include <string>
#include "dodo/types/Base.hpp"


namespace dodo
{
    namespace components
    {
        namespace types
        {

            struct PortKey :
                public dodo::types::Base<std::string>
            {
                PortKey() = default;


                PortKey(PortKey::value_type s) :
                   Base(s)
                {};


                friend std::string toString(const PortKey &id)
                {
                    std::ostringstream oss;
                    oss << "Port: " << id;
                    return oss.str();
                }
            };


        }
    }
}
