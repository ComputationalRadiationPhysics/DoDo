#pragma once

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
        public T_Extensions...
    {
        HardwareAbstraction(T_Extension&&... extensions) :
            T_Extensions(extensions)...
        {}
    };

} /* hardware */
} /* model */
} /* dodo */

