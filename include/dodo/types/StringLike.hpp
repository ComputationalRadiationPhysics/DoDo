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
            StringLike(const StringLike&) = default;
            StringLike(StringLike&&) = default;
            StringLike& operator=(const StringLike&) = default;
            StringLike& operator=(StringLike&&) = default;
            ~StringLike() = default;

            inline auto size()
            {
                return this->value.size();
            }


            StringLike(Base::value_type s) :
                Base(s)
            {};


            friend std::string toString(const StringLike &id)
            {
                std::ostringstream oss;
                oss << id;
                return oss.str();
            }



            friend
            bool
            operator==(
                const StringLike &lhs,
                const StringLike &rhs
            )
            {
                return lhs.value == rhs.value;
            }

            friend
            bool
            operator<(
                const StringLike &lhs,
                const StringLike &rhs
            )
            {
                return lhs.value < rhs.value;
            }
        };

        std::ostream& operator<<(
            std::ostream & ostream1,
            const StringLike & s
        )
        {
            return ostream1 << s.value;
        }



    }
}

namespace boost{
    template<>
    dodo::types::StringLike lexical_cast(const std::string & s) {
        return dodo::types::StringLike(s);
    }
} /* boost */
