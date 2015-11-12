#include <iostream>

#include <chunky.hpp>

int main()
{
    using Speed                  = chunky::physical::attributes::Speed;
    using SpeedGraph             = chunky::graph::AttributeGraph<Speed>;
    SpeedGraph spg;

    auto nodeSpeedProperty = spg.createProperty(Speed{3000.});

    spg.addVertex(nodeSpeedProperty.clone());
    return 0;
}
