#pragma once

namespace dodo
{
    namespace hardware
    {
        namespace attributes
        {

            struct EnergyLevel
            {
                int value;


                EnergyLevel
                merge(const EnergyLevel &other)
                {
                    return {value + other.value};
                }
            };


        }
    }
}
