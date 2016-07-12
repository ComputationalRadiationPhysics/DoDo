#pragma once

#include <string>
#include <sstream>

namespace dodo
{
namespace types
{

    template<typename T>
    struct Base
    {
        using value_type = T;
        value_type value;

        Base( ) = default;

        Base( const Base & ) = default;

        Base( Base && ) = default;

        Base & operator=( const Base & ) = default;

        Base & operator=( Base && ) = default;

        virtual ~Base( ) = default;


        Base(T t) :
            value(t)
        {};


        operator value_type() const
        {
            return value;
        };



        bool
        operator<( const Base & other ) const
        {
            return value < other.value;
        }


        friend
        std::ostream &
        operator<<(
            std::ostream & stream,
            const Base & base
        )
        {
            stream << base.value;
            return stream;
        }


        friend
        std::string
        toString( const Base & b )
        {
            std::ostringstream oss;
            oss << b;
            return oss.str( );
        }


    };

} /* types */
} /* dodo */
