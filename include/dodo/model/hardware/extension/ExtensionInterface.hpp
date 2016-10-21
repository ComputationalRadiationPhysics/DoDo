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
        void
        addPropertyToDPWriter(
            boost::dynamic_properties &,
            std::list< std::shared_ptr< void > > &
        ) = 0;

    protected:
        virtual
        std::size_t
        countPropertiesInternal( ) = 0;


        virtual ~ExtensionInterface(){}

    };

} /* extension */
} /* hardware */
} /* model */
} /* dodo */
