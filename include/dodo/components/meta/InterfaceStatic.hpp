#pragma once


#include <vector>
#include <map>
#include <set>
#include <exception>

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/string.hpp>
#include <boost/mpl/has_key.hpp>

#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/hana.hpp>


#include "dodo/components/types.hpp"
#include "dodo/components/meta/detail/InitRuntimeMap.hpp"
#include "dodo/components/meta/detail/InitRuntimeSeq.hpp"
#include "dodo/components/meta/traits.hpp"
#include "dodo/components/meta/Interface.hpp"


namespace dodo
{
    namespace components
    {
        namespace meta
        {


            /**
             * Main reason for this interface at the time:
             * Enforce existance of certain traits.
             *
             * Inheriting from "Interface" is not 100% necessary,
             * the same could be done using multiple inheritance for the derived class
             */
            template<typename T_Derived>
            class InterfaceStatic :
                public Interface
            {
            public:
                using InPorts = traits::InPortMap<T_Derived>;
                using OutPorts = typename traits::OutPortMap<T_Derived>::value;
                using Role = typename traits::Role<T_Derived>::value;
                using Complexity = typename traits::Complexity<T_Derived>::value;

                decltype(InPorts::init()) inPortM;


                InterfaceStatic()
                {
                    inPortM = InPorts::init();
//                    testCorrectMapNablas();
//                    testNoIdenticalPorts();
                    detail::initRuntimeMap<typename InPorts::value>(inPortMap);
                    detail::initRuntimeMap<OutPorts>(outPortMap);
                    detail::initRuntimeSeq<Role>(roles);
                }


            private:
                constexpr void testNoIdenticalPorts()
                {
                    boost::fusion::for_each(
                        InPorts(),
                        [&](auto i)
                        {
                            using Elem = decltype(i);
                            using Key = typename Elem::first;
                            static_assert(
                                !boost::mpl::has_key<OutPorts, Key>::type::value,
                                "Input and Output ports may not have the same name!"
                            );
                        }
                    );
                }


                constexpr void testCorrectMapNablas()
                {
                    checkStringToInt<InPorts>();
                    checkStringToInt<OutPorts>();
                }


                template<typename T>
                constexpr void checkStringToInt()
                {
                    boost::fusion::for_each(
                        T(),
                        [](auto i)
                        {
                            using Elem = decltype(i);
                            static_assert(
                                std::is_same<
                                    typename Elem::first::value_type,
                                    boost::mpl::string<'a'>::value_type
                                >::value,
                                "Not a boost::mpl::string!"
                            );

                            static_assert(
                                std::is_same<
                                    typename Elem::second::value_type,
                                    boost::mpl::int_<1>::value_type
                                >::value,
                                "Not a boost::mpl::int!"
                            );
                        }
                    );
                }

            };


        }
    }
}

