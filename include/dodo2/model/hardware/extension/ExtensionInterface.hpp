#pragma once

#include <memory>

namespace dodo
{
namespace model
{
namespace hardware
{
namespace extension
{

    struct ExtensionInterface
    {
        virtual
        void addPropertyToDPWriter(
            boost::dynamic_properties &,
            std::list< std::shared_ptr< void > > &
        )
        { }

        virtual ~ExtensionInterface(){}

    };

} /* extension */
} /* hardware */
} /* model */
} /* dodo */
