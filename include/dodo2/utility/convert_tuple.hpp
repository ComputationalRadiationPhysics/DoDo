#pragma once


#include <boost/hana.hpp>


namespace dodo
{
    namespace utility
    {

        template<typename T_OldTuple, typename T_NewTuple>
        void
        convertTuple(const T_OldTuple &oldTuple, T_NewTuple &newTuple)
        {
            boost::hana::for_each(
                newTuple,
                [&oldTuple](auto &i)
                {
                    using ElemType = typename std::decay<decltype(i)>::type;
                    constexpr size_t srcIndex = utility::getTupleIndex<T_OldTuple, ElemType>();
                    i = std::get<srcIndex>(oldTuple);
                }
            );
        }

    }
}
