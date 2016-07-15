#pragma once

#include <set>
#include <string>

#include <boost/property_map/dynamic_property_map.hpp>

#include <dodo2/utility/tree_id.hpp>
#include <dodo2/graph/SimpleBGL.hpp>


namespace dodo
{
namespace utility
{
    class PropertyManager
    {
        boost::dynamic_properties properties;
        std::set<std::string> registeredNames;

    public:
        template <typename T>
        using MapType = boost::associative_property_map<T>;

        template<typename T_PropMap>
        void
        registerProperty(const std::string pName, const T_PropMap & p)
        {
            if( registeredNames.find( pName ) != registeredNames.end( ) )
            {
                throw std::runtime_error(
                    "Cannot register property " +
                    pName +
                    ": a property with that name already exists!"
                );
            }
            registeredNames.emplace(pName);
            properties.property(pName, p);
        }

        template<typename T>
        T
        get(
            const std::string & propName,
            const utility::TreeID & id
        ) const
        {
            return boost::get<T>(propName, properties, id);
        }

        template<typename T>
        T
        get(
            const std::string & propName,
            const graph::SimpleBGL::EdgeID id
        ) const
        {
            return boost::get<T>(propName, properties, id);
        }

        template<typename T>
        void
        set(
            const std::string & propName,
            const utility::TreeID & id,
            T property
        )
        {
            put( propName, properties, id, property );
        }

        template<typename T>
        void
        set(
            const std::string & propName,
            const graph::SimpleBGL::EdgeID id,
            T property
        )
        {
            put( propName, properties, id, property );
        }
    };

} /* utility */
} /* dodo */

