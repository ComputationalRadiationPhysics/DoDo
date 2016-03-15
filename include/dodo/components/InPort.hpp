#pragma once


#include "OutPort.hpp"
#include "Port.hpp"


namespace dodo
{
    namespace components
    {

        class InPort :
            public Port
        {
        public:
            //InPort() = default;
            InPort()
            {}


            /**
             * This should probably be refactored to the ringbuffer?
             * Instead, the port should instantly put everything into
             * the buffer and increase the cunksbeforeReady counter
             */
            auto get()
            {}


            /**
             * Return true if the number of current chunks in the
             * buffer is higher or equal to the necessary number
             * of chunks
             */
            bool isReady()
            {
                /**
                 * If (chunks I am responsible for in the buffer >= chunksnecessary)
                 *     return true;
                 * else
                 *      return false;
                 */
                return true;
            }
        };


    }
}
