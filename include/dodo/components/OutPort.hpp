#pragma once


#include "Port.hpp"


namespace dodo
{
    namespace components
    {

        class OutPort :
            public Port
        {
        public:
            template<typename Chunk>
            void put(const Chunk &&)
            {}

        };

    }
}
