#pragma once

#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>


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
        public T_Extensions...

    {
//        HardwareAbstraction(T_Extensions... extensions) :
//            T_Extensions(extensions)...
//        {}
        HardwareAbstraction() :
            T_Extensions()...
        {}
    };

} /* hardware */
} /* model */
} /* dodo */

