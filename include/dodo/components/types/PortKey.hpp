#pragma once

#include <sstream>
#include <string>
#include "Base.hpp"


namespace dodo
{
    namespace components
    {
        namespace types
        {

            struct PortKey :
                public Base<std::string>
            {
                PortKey() = default;


                PortKey(Base::value_type s) :
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
