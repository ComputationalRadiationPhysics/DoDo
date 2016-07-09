#pragma once


#include <dodo2/utility/tree_id.hpp>
#include <map>
#include "IMap.hpp"
#include "IProperty.hpp"


namespace dodo{
    namespace model{
        namespace hardware{
            namespace property{

            template<typename T>
            struct Map : public IMap{
                using Internal_T = T;
                std::map<utility::TreeID, T> propMap;
            };
            }

        }
    }
}

