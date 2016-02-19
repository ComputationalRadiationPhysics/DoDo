#pragma once


#include <memory>


class OutPort;
class InPort;


namespace dodo
{
namespace components
{

    class Connection
    {
        std::weak_ptr<OutPort> from;
        std::weak_ptr<InPort> to;

    };

}
}


