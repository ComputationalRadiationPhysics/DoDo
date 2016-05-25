#pragma once
#include <vector>
#include <memory>

#include <boost/fusion/include/for_each.hpp>



namespace dodo
{
    namespace hardware
    {
        namespace detail
        {

            /** Open typeswitch
             *
             * Circumvents the Expression Problem, without the closed-ness that
             * comes with the usual solution (Visitor design pattern). The
             * dynamic_cast isn't as fast as Visitor-based solutions, but it
             * should not matter too much, since the number of different classes
             * is expected to remain low. Also, this code is mostly run once
             * during the initialization phase
             *
             * Usage: The arbiter has to be parametrized with a fusion::list
             * (or sth that models the Sequence concept). To the apply method,
             * pass the name of a functor that should be executed. This functor
             * needs to be a template that is specialized for each type that is
             * passed to apply. Failure to do so will cause a run-time exception
             */
            template<
                typename T_Base,
                typename T_DerivedSeq
            >
            struct Arbiter
            {
                /** TODO: Think about replacing this structure with sth. faster
                 * see Alexandrescu's Generic Visitor (Modern C++ Design,
                 * Section 10.5)
                 * See also Abrahams' "C++ Template Metaprogramming" Section 9.5
                 * -> Generation of multiple Inheritance
                 */

                /** Worker function
                 *
                 * Will call the operator() on the functor that is specialized
                 * with a type derived from the base class
                 */
                template< template< typename > class T >
                static void apply( T_Base * b )
                {
                    boost::fusion::for_each(
                        T_DerivedSeq{ },
                        [ & ]( auto i )
                        {
                            using I = decltype( i );
                            if( I * a = dynamic_cast< I * >( b ) )
                            {
                                T< I >( )( );
                                return;
                            }
                        }
                    );
                    throw std::runtime_error(
                        "The supplied functor was not specialized\
                        for one of the types"
                    );
                }


                /** Convenience function to wrap std::shared_ptr
                 */
                template< template< typename > class T >
                static void apply( std::shared_ptr< T_Base > & b )
                {
                    apply< T >( b.get( ) );
                }


                /** Convenience function to wrap iterable sequences
                 */
                template<
                    template< typename > class T,
                    typename T_Sequence,
                    typename = typename T_Sequence::iterator //SFINAE
                >
                static void apply( T_Sequence& sequence )
                {
                    for( auto& i : sequence )
                    {
                        apply< T >( i );
                    }
                }
            };

        }
    }
}
