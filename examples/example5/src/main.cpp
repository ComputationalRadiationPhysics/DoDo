#include <iostream>

#include <dout/dout.hpp>

#include <dodo.hpp>


int main( )
{
    using namespace dodo::graph;
    HWPropertyInterface* x = new PolyProperty();
    std::cout << x->toString();

    return 0;
}
