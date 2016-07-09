#pragma once


#include <dodo2/graph/TreeIDGraph.hpp>
#include <dodo2/graph/InterconnectGraph.hpp>
#include <dodo2/model/hardware/property/VertexName.hpp>
#include <dodo2/model/hardware/property/IMap.hpp>
#include <dodo2/model/hardware/property/Map.hpp>
#include <boost/variant.hpp>
#include <dodo2/model/hardware/property/VertexType.hpp>
#include <dodo2/model/hardware/property/EdgeName.hpp>
#include <dodo2/utility/tuple_index.hpp>
#include "PropertyManager.hpp"


namespace dodo
{
    namespace model{

        namespace hardware
        {
            using MemoryHierarchyGraph = graph::TreeIDGraph;
            using ConsistsOfGraph = graph::TreeIDGraph;

            class IHardwareAbstraction
            {
                // These graphs represent the STRUCTURE, not the attributes
                ConsistsOfGraph cog;

                std::map<
                    graph::TreeIDGraph::TreeID,
                    std::string
                > id2name;
                PropertyManager::MapType < decltype( id2name ) > nameMap;
            protected:
                PropertyManager propertyManager;
            public:

                IHardwareAbstraction() :
                    nameMap(id2name)
                {
                    propertyManager.registerProperty("VertexName", nameMap);
                }

                template<typename T>
                T
                getProperty(
                    const std::string & propName,
                    const utility::TreeID & id
                ) {
                    return propertyManager.get(propName, id);
                }


                template<typename T>
                void
                setProperty(
                    const std::string & propName,
                    const utility::TreeID & id,
                    T property
                ) {
                    propertyManager.set( propName, id, property );
                }



            };

        }
    }
}

