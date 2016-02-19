#pragma once


#include <vector>
#include "Connection.hpp"


namespace dodo
{
namespace components
{
    class Port
    {
    protected:
        std::vector<Connection> connections;
    };

}
}
