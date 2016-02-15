#pragma once

namespace dodo
{
namespace graph
{

    class HWPropertyInterface
    {
    public:
        std::string name;

        HWPropertyInterface() = delete;

    protected:
        HWPropertyInterface(std::string name) : name(name){};
    public:

        virtual std::string toString() = 0;
    };


    class PolyProperty : public HWPropertyInterface
    {
    public:
        using HWPropertyInterface::name;

        PolyProperty() :
            HWPropertyInterface("Compute")
        {}

        virtual std::string toString() override
        {
            return name;
        }
    };
}
}
