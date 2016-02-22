#pragma once


#include <memory>




namespace dodo
{
namespace components
{

    class OutPort;
    class InPort;

    class Connection
    {
        std::weak_ptr<OutPort> from;
        std::weak_ptr<InPort> to;

    public:
        Connection(
            const std::weak_ptr<OutPort> &from,
            const std::weak_ptr<InPort> &to
        ) :
            from(from),
            to(to)
        {
        }
    };

}
}


