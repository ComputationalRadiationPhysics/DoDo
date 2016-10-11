#pragma once

#include <string>

#include <dodo2/model/data/DataDomain.hpp>


namespace dodo{
namespace model{
namespace routine{


    class InData
    {
    public:
        data::DataDomain::DataID id;
        std::string domain;
    };

    using OutData = InData;


} /* routine */
} /* model */
} /* dodo */


