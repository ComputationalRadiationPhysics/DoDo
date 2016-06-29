#pragma once

namespace dodo{
    namespace hardware{


        class Bandwidth
        {
        public:
            size_t bw;
            Bandwidth(size_t b):
                bw(b)
            {}

        };

        std::ostream& operator<<(std::ostream& ostream1, const Bandwidth& bw)
        {
            return ostream1 << bw.bw;
        }
    }
}

