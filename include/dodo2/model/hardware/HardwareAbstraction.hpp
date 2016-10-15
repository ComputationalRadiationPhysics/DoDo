#pragma once

#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/model/hardware/extension/ExtensionInterface.hpp>


namespace dodo{
namespace model{
namespace hardware{

    /**
     * Uses several extensions that all have to inherit virtually
     * from the HardwareAbstractionBase base class.
     * In its interface, this class will have available all the interface
     * methods from its bases. Thus, it acts as the nicely accessible interface.
     */
    template<typename... T_Extensions>
    struct HardwareAbstraction :
        public virtual HardwareAbstractionBase,
        public virtual T_Extensions...
    {
    private:
        template<
            typename T,
            typename... Ts
        >
        void fillPropMap(
            boost::dynamic_properties & dp,
            std::list< std::shared_ptr< void > > & freeList
        )
        {
            T::addPropertyToDPWriter( dp, freeList );
            fillPropMap< Ts... >( dp, freeList );
        }

        template< typename... Ts >
        auto fillPropMap(
            boost::dynamic_properties &,
            std::list< std::shared_ptr< void > > &
        )
        -> typename std::enable_if< sizeof...( Ts ) == 0 >::type
        { }

        template<typename T_Graph>
        void
        writeTreeIDGraph(
            const T_Graph & graph,
            std::string path,
            boost::dynamic_properties & dp
        )
        {
            std::ofstream ofs;
            ofs.open(path);
            write_graphml(ofs, *(graph.graph), dp );
            ofs.close();
        }

        template<typename M>
        auto
        addLocalMap(
            std::string name,
            M & localMap,
            boost::dynamic_properties & dp,
            std::list< std::shared_ptr< void > > & freeList
        )
        -> void
        {
            auto indexMap = createIndexMap(
                localMap,
                cog
            );
            freeList.push_back( indexMap );
            boost::associative_property_map< typename std::decay< decltype( *indexMap ) >::type >
                associativeMap( *indexMap );
            dp.property(
                name,
                associativeMap
            );
        }


        template<
            typename TProp,
            typename... T_Props
        >
        auto
        countOtherProperties( std::size_t & s )
        -> void
        {
            s += TProp::countPropertiesInternal( );
            countOtherProperties< T_Props... >( s );
        }

        template<typename... T_Props>
        auto
        countOtherProperties( std::size_t & )
        -> typename std::enable_if< sizeof...( T_Props ) == 0 >::type
        { }


    public:

        auto
        countProperties()
        -> std::size_t
        {
            std::size_t s = countPropertiesBase( );
//            countOtherProperties< T_Extensions... >( s );
            boost::mpl::for_each< boost::mpl::vector<T_Extensions*...> >(
                [&s, this]( auto i )
                {
                    s += this->std::decay< decltype( *i ) >::type::countPropertiesInternal( );
                }
            );
            return s;
        }

        HardwareAbstraction() :
            T_Extensions()...
        {
            boost::mpl::for_each< boost::mpl::vector<T_Extensions*...> >(
                []( auto i )
                {
                    static_assert(
                        std::is_base_of<
                            dodo::model::hardware::extension::ExtensionInterface,
                            typename std::decay< decltype( *i ) >::type
                        >::value,
                        "Extension does not inherit from ExtensionInterface"
                    );
                }
            );


        }

        void
        writeAllGraphs( std::string path_base )
        {
            std::list< std::shared_ptr< void > > freeList;
            boost::dynamic_properties dp;

            addLocalMap("VertexType", internal_typeMap, dp, freeList );
            addLocalMap("VertexName", internal_nameMap, dp, freeList );
            dp.property("TreeID", boost::get(boost::vertex_bundle, *cog.graph));
            dp.property( "EdgeName", edgeNameMap );

            fillPropMap< T_Extensions... >(
                dp,
                freeList
            );

            writeTreeIDGraph(
                cog,
                path_base + "consists_of_graph.graphml",
                dp
            );

            writeTreeIDGraph(
                mhg,
                path_base + "memory_hierarchy_graph.graphml",
                dp
            );

            writeTreeIDGraph(
                ig,
                path_base + "interconnect_graph.graphml",
                dp
            );
        }
        void
        writeAllMaps( std::string path_base )
        {
            std::list< std::shared_ptr< void > > freeList;
            boost::dynamic_properties dp;


            // TODO: implement
            for(std::string m : propertyManager.registeredNames)
            {
                std::cout << m << std::endl;
            }

        }

    };


} /* hardware */
} /* model */
} /* dodo */

