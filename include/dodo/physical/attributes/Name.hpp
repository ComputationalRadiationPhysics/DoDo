#pragma once

namespace dodo
{
namespace physical
{
namespace attributes
{
    struct Name
    {
         std::string value;

        Name merge(const Name& b) const
        {
            return { b.value + value };
        }

        auto toString() const
        {
            return value;
        }
    };

}
}
}
