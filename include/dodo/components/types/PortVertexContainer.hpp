#pragma once


#include <string>
#include <stdlib.h>


namespace types{

    enum PortVertexTypes
    {
        UNDEFINED = 0,
        ATOMIC_IN,
        ATOMIC_OUT,
        ATOMIC_STEP,
        HIERARCHICAL_IN,
        HIERARCHICAL_OUT
    };

    struct PortVertexContainer
    {
        PortVertexTypes t;
        PortVertexContainer( ) :
            t( PortVertexTypes::UNDEFINED )
        { }
        PortVertexContainer( PortVertexTypes _t ) :
            t( _t )
        { }
    };
}

namespace boost{

    template<>
    std::string
    lexical_cast( const types::PortVertexTypes & m )
    {
        return std::to_string( static_cast< int >( m ) );
    }

    template< >
    types::PortVertexTypes
    lexical_cast( const std::string & s )
    {
        return static_cast< types::PortVertexTypes >( atoi( s.c_str( ) ) );
    }
}
