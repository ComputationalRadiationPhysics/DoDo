#pragma once

namespace dodo{
namespace utility{


class UniqueID{
    size_t current;
    UniqueID() : current(0) {};

public:
    static size_t get(){
        static UniqueID instance;
        return instance.current++;
    }
};


} /* utility */
} /* dodo */
