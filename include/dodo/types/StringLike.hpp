#pragma once


#include <string>
#include <sstream>
#include "dodo/types/Base.hpp"


namespace dodo
{
    namespace types
    {

        struct StringLike :
            public Base<std::string>
        {
            StringLike() = default;


            StringLike(Base::value_type s) :
                Base(s)
            {};


            friend std::string toString(const StringLike &id)
            {
                std::ostringstream oss;
                oss << id;
                return oss.str();
            }
        };

    }
}
