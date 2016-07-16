#pragma once

#include <set>
#include <string>

#include <boost/property_map/dynamic_property_map.hpp>

namespace dodo
{
namespace utility
{
    class PropertyManager
    {
        boost::dynamic_properties properties;
        std::set< std::string > registeredNames;

    public:
        template< typename T >
        using MapType = boost::associative_property_map< T >;

        template<typename T_PropMap>
        auto
        registerProperty(
            const std::string pName,
            const T_PropMap & p
        ) ->  void
        {
            if( registeredNames.find( pName ) != registeredNames.end( ) )
            {
                throw std::runtime_error(
                    "Cannot register property " +
                    pName +
                    ": a property with that name already exists!"
                );
            }
            registeredNames.emplace( pName );
            properties.property(
                pName,
                p
            );
        }


        template<
            typename T,
            typename I
        >
        auto
        get(
            const std::string & propName,
            const I id
        ) const -> T
        {
            return boost::get< T >(
                propName,
                properties,
                id
            );
        }


        template<
            typename T,
            typename I
        >
        auto
        set(
            const std::string & propName,
            const I & id,
            T property
        ) -> void
        {
            put(
                propName,
                properties,
                id,
                property
            );
        }


//        template< typename T >
//        auto
//        set(
//            const std::string & propName,
//            const graph::SimpleBGL::EdgeID id,
//            T property
//        ) -> void
//        {
//            put(
//                propName,
//                properties,
//                id,
//                property
//            );
//        }
//
//
//        template< typename T >
//        auto
//        get(
//            const std::string & propName,
//            const utility::TreeID & id
//        ) const -> T
//        {
//            return boost::get< T >(
//                propName,
//                properties,
//                id
//            );
//        }
//    };

} /* utility */
} /* dodo */

