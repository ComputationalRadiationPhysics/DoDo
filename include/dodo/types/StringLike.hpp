#pragma once


#include <string>
#include <sstream>
#include "dodo/types/Base.hpp"


namespace dodo
{
namespace types
{

    struct StringLike :
        public Base< std::string >
    {
        StringLike( ) = default;

        StringLike( const StringLike & ) = default;

        StringLike( StringLike && ) = default;

        StringLike & operator=( const StringLike & ) = default;

        StringLike & operator=( StringLike && ) = default;

        ~StringLike( ) = default;


        inline
        auto
        size( ) const
        {
            return this->value.size( );
        }


        explicit
        StringLike( Base::value_type s ) :
            Base( s )
        { }


        friend
        std::string
        toString( const StringLike & id )
        {
            std::ostringstream oss;
            oss << id;
            return oss.str( );
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
    }; /* StringLike */

    std::ostream&
    operator<<(
        std::ostream &,
        const StringLike &
    );

    std::ostream&
    operator<<(
        std::ostream & ostream1,
        const StringLike & s
    )
    {
        return ostream1 << s.value;
    }


} /* types */
} /* dodo */

namespace boost
{
    template<>
    dodo::types::StringLike
    lexical_cast( const std::string & s )
    {
        return dodo::types::StringLike( s );
    }
} /* boost */
