#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <exception>

#include <boost/lexical_cast.hpp>


namespace dodo{
namespace model{
namespace worker{
namespace property{
    enum class NodeType
    {
        ADDRESS_SPACE,
        WORKER,
        _count
    };


    std::ostream & operator<<(
        std::ostream & ostream1,
        const NodeType & n
    )
    {
        switch( n )
        {
            case NodeType::ADDRESS_SPACE:
                ostream1 << "AddressSpace";
                break;
            case NodeType::WORKER:
                ostream1 << "Worker";
                break;
            default:
                throw std::runtime_error( "Unknown NodeType!" );
        }
        return ostream1;
    }


    std::string to_string( const NodeType & n )
    {
        std::stringstream ss;
        ss << n;
        return ss.str( );
    }

} /* property */
} /* worker */
} /* model */
} /* dodo */

namespace boost
{
    template<>
    dodo::model::worker::property::NodeType
    lexical_cast( const std::string & s )
    {
        if( s == "AddressSpace" )
            return dodo::model::worker::property::NodeType::ADDRESS_SPACE;
        if( s == "Worker" )
            return dodo::model::worker::property::NodeType::WORKER;
        throw std::runtime_error( "Unknown NodeType!" );
    }
} /* boost */

