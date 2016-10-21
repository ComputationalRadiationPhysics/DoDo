#pragma once

#include <string>
#include <iostream>

#include <dodo/model/data/DataDomain.hpp>


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

    std::ostream &
    operator<<(
        std::ostream &,
        const InData &
    );
    std::istream& operator>>(
        std::istream &,
        InData &
    );


    std::ostream &
    operator<<(
        std::ostream & ostream1,
        const InData & d
    )
    {
        ostream1 << d.id << std::endl;
        ostream1 << d.domain << std::endl;
        return ostream1;
    }

    std::istream& operator>>(
        std::istream & stream,
        InData & d
    )
    {
        stream >> d.id;
        stream.get();
        stream >> d.domain;
        return stream;
    }



} /* routine */
} /* model */
} /* dodo */


