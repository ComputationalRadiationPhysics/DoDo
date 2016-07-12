#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <exception>

#include <boost/lexical_cast.hpp>


namespace dodo{
namespace model{
namespace hardware{
namespace property{
    enum class VertexType
    {
        COMPUTE,
        MEMORY,
        CACHE,
        STRUCTURAL,
        INTERCONNECT,
        _count
    };


    std::ostream & operator<<(
        std::ostream & ostream1,
        const VertexType & n
    )
    {
        switch( n )
        {
            case VertexType::COMPUTE:
                ostream1 << "Compute";
                break;
            case VertexType::MEMORY:
                ostream1 << "Memory";
                break;
            case VertexType::CACHE:
                ostream1 << "Cache";
                break;
            case VertexType::STRUCTURAL:
                ostream1 << "Structural";
                break;
            case VertexType::INTERCONNECT:
                ostream1 << "Interconnect";
                break;
            default:
                throw std::runtime_error( "Unknown VertexType!" );

        }
        return ostream1;
    }


    std::string to_string( const VertexType & n )
    {
        std::stringstream ss;
        ss << n;
        return ss.str( );
    }

} /* property */
} /* hardware */
} /* model */
} /* dodo */

namespace boost
{
    template<>
    dodo::model::hardware::property::VertexType
    lexical_cast( const std::string & s )
    {
        if(s == "Compute")
            return dodo::model::hardware::property::VertexType::COMPUTE;
        if(s == "Memory")
            return dodo::model::hardware::property::VertexType::MEMORY;
        if(s == "Cache")
            return dodo::model::hardware::property::VertexType::CACHE;
        if(s == "Structural")
            return dodo::model::hardware::property::VertexType::STRUCTURAL;
        if(s == "Interconnect")
            return dodo::model::hardware::property::VertexType::INTERCONNECT;
        throw std::runtime_error("Unknown VertexType!");
    }
} /* boost */

