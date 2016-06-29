#pragma once


#include <dodo/utility/tree_id.hpp>
#include <dodo/hardware/detail/Arbiter.hpp>


namespace dodo
{
    namespace hardware
    {

        template<typename T>
        struct ConsistsOf
        {
            void operator()(T& t, graph::TreeIDGraph& g)
            {
                g.addVertex(t.id);
            }
        };

        struct HWNode
//            :
//            public std::enable_shared_from_this< HWNode >
        {
            using ID = dodo::utility::TreeID;
//            using ResourceMap = std::weak_ptr<
//                std::map<
//                    ID,
////                    std::shared_ptr< HWNode >
//                    HWNode*
//                >
//            >;
//            ResourceMap resources;

            ID id;


            HWNode( ID id ) :
                id { id }
            {}

//            template<typename T>
//            virtual void accept(AbstractDispatcher<T> &) = 0;

            template< typename T >
            std::shared_ptr< T > createChild( )
            {
                auto newNode = std::make_shared< T >(
                    id.genChildID( )
                );
                return newNode;
            }



        };

        template<typename ... T_HWTypes>
        class HWDescription
        {
        public:
            using ID = HWNode::ID;
//            using ResourceMap = HWNode::ResourceMap::element_type;
            using ResourceMap = std::map<ID, std::shared_ptr<HWNode>>;
            using HWTypes = boost::fusion::vector<T_HWTypes...>;

        private:
            std::shared_ptr< ResourceMap > resources;
            graph::TreeIDGraph g;
            ID rootID;
            boost::any rootNode;



            HWDescription( ) :
                resources { std::make_shared< ResourceMap >( ) },
                rootID { ID( ) }
            { }


            template< typename T >
            void initImpl( )
            {
                rootNode = new T(rootID);
//                ( *resources )[rootID] =
//                    std::make_shared< T >(
//                        rootID,
//                        resources
//                    );
//
//                ( *resources )[rootID]->consistsOf( g );

//                auto a= g.getVertices();
//                std::cout << std::distance(a.first, a.second) << std::endl;
//                utility::TreeID testID = utility::TreeID().genChildID();
//                auto child1 = testID.genChildID();
//                std::cout << "Parent: " << testID << " Child " << child1 << " Parent from child's view: " << child1.getParentID() << std::endl;

            }



        public:

//            template<template<typename> class T>
//            static void apply(HWNode* b)
//            {
//                boost::fusion::for_each(
//                    HWTypes{},
//                    [&](auto i)
//                    {
//                        using I = decltype(i);
//                        if(I* a = dynamic_cast<I*>(b)){ T<I>()(); return; }
//                    }
//                );
//            }
//
//            template<template<typename> class T>
//            static void apply(std::vector<HWNode*> v)
//            {
//                for(auto i : v)
//                {
//                    apply<T>(i);
//                }
//            }

            template<template<typename> class T, typename T_Input>
            static void apply(T_Input input)
            {
                detail::Arbiter<HWNode, HWTypes >::template apply<T>(input);
            };

            template< typename T >
            static HWDescription init( )
            {
                HWDescription h;
                h.initImpl< T >( );
                return h;
            }
        };


    } /* hardware */

} /* dodo */
