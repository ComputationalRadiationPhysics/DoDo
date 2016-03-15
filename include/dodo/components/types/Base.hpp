#pragma once

#include <string>
#include <sstream>

namespace dodo
{
    namespace components
    {
        namespace types
        {

            template<typename T>
            struct Base
            {
                using value_type = T;
                value_type value;

                Base() = default;


                Base(T t) :
                    value(t)
                {};


                operator value_type() const
                {
                    return value;
                };


//                operator value_type*() { return &value; };

                bool operator<(const Base &other) const
                {
                    return value < other.value;
                }


                friend std::ostream &operator<<(std::ostream &stream, const Base &base)
                {
                    stream << base.value;
                    return stream;
                }


                friend std::string toString(const Base &b)
                {
                    std::ostringstream oss;
                    oss << b;
                    return oss.str();
                }

                virtual ~Base() = default;

            };

        }
    }
}
