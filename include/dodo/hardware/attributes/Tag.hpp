#pragma once


namespace dodo
{
    namespace hardware
    {
        namespace attributes
        {

            struct Tag
            {
                enum class Tags
                {
                    Unknown, Switch, Compute, Memory, Structural
                } value;


                Tag
                merge(const Tag &t)
                {
                    if (t.value != value)
                    {
                        throw std::runtime_error("Cannot merge Elements with different Tags");
                    }
                    return t;
                }


                std::string toString()
                {
                    switch (value)
                    {
                        case Tags::Unknown:
                            return "Unknown";
                        case Tags::Switch:
                            return "Switch";
                        case Tags::Compute:
                            return "Compute";
                        case Tags::Memory:
                            return "Memory";
                        default:
                            return "default";
                    }

                }

            };

        }
    }
}
