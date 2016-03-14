#pragma once


namespace dodo
{
namespace components
{
namespace meta
{
namespace detail
{
/**
 * Helper function to convert a boost::fusion compile-time map
 * into a runtime-map.
 *
 * @tparam CompileTimeMap should have types similar to
 *         boost::mpl::int_<T> as keys, so that the keys can
 *         be properly generated
 */
template<typename CompileTimeMap>
void initRuntimeMap(std::map<types::PortKey::value_type, int> &m)
{
    boost::fusion::for_each(
        CompileTimeMap(),
        [&](auto i)
        {
            using Elem = decltype(i);
            using KeyType = typename Elem::first;
            using ValueType = typename Elem::second;
            m[boost::mpl::c_str<KeyType>::value] = ValueType().value;
        }
    );
}

}
}
}
}

