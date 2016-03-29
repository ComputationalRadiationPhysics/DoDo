#include <iostream>

#include <dout/dout.hpp>

#include <dodo.hpp>

#include "cluster_definition.hpp"

int main( )
{
    decltype(auto) dout = dout::Dout::getInstance();
    dout.setVerbosity(dout::Flags::WARN | dout::Flags::INFO | dout::Flags::STAT | dout::Flags::ERROR);
    //dout.addVerbosity(dout::Flags::DEBUG);

    HardwareGraph<CPUVertex> g;


//    auto combinedGraph = g.extractData(
//        [](auto& i)
//        {
//            using namespace dodo::hardware::attributes;
//            bool keep = false;
//            // keep |= i.template getProperty<Tag>().value == Tag({Tag::Tags::Switch}).value;
//            keep |= i.template getProperty<Tag>().value == Tag({Tag::Tags::Compute}).value;
//            keep |= i.template getProperty<Name>().value == "Core";
//            keep |= i.template getProperty<Name>().value == "Gigabit EthernetSwitch";
//            return keep;
//        }
//    );
//
//
//    constexpr auto edgeLabels = std::make_tuple(
//        std::make_tuple("Bandwidth", dodo::hardware::attributes::Bandwidth())
//    );
//    auto vertexLabels = std::make_tuple(
//        std::make_tuple("Tag", dodo::hardware::attributes::Tag()),
//        std::make_tuple("Name", dodo::hardware::attributes::Name()),
//        std::make_tuple("Clock Frequency", dodo::hardware::attributes::Speed())
//    );
//
//    std::ofstream graphFile ("graph.graphml");
//    dodo::hardware::writeGraph(
//        combinedGraph,
//        vertexLabels,
//        edgeLabels
//        , graphFile
//    );
//    graphFile.close();

/*
 *    Nice usage to build dependency-graph:
 *
 *    class SourceMeta
 *    {
 *      //...
 *    }
 *
 *    Network net;
 *
 *    auto source = net.genMetaComponent<SourceMeta>();
 *
 *    source.set
 *
 *    TransportMeta transport1, transport2;
 *    ProcessingMeta processing;
 *    SinkMeta sink;
 *
 *    DependencyGraph g = source -> transport1 -> Farm(processing, 5) -> transport2 -> sink
 *
 *
 *
 */

    std::cout << g.consistsOfGraph << std::endl;

    return 0;
}
