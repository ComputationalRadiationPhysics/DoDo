#include <iostream>
#include <boost/graph/graphml.hpp>
#include <dodo/components/dependency/HierarchicalComponent.hpp>
#include <dodo.hpp>
#include <dodo/hardware/HardwareAbstraction.hpp>



enum ThreadGraphNodeType{
    PROCESS,
    THREAD,
    _count
};


int main( )
{
    dodo::hardware::HardwareAbstraction hwa;

    auto rootNode = hwa.addRoot("Hypnos", dodo::hardware::NodeType::STRUCTURAL);
    auto switch1 = hwa.addNode("IB-Switch1", dodo::hardware::NodeType::INTERCONNECT, rootNode);
    auto switch2 = hwa.addNode("IB-Switch2", dodo::hardware::NodeType::INTERCONNECT, rootNode);
    auto switch3 = hwa.addNode("IB-Switch3", dodo::hardware::NodeType::INTERCONNECT, rootNode);
    auto ibcables1 = hwa.addInterconnectBidirectional(switch1, switch2, "IB");
    auto ibcables2 = hwa.addInterconnectBidirectional(switch1, switch3, "IB");

    constexpr unsigned nMachines = 4;
    constexpr unsigned nSockets = 2;
    constexpr unsigned nCores = 4;

    std::vector<dodo::utility::TreeID> k20Nodes(nMachines);
    for(unsigned machine_i = 0; machine_i < nMachines; ++machine_i)
    {
        auto machine = hwa.addNode(
            "Kepler00" + std::to_string( machine_i ),
            dodo::hardware::NodeType::STRUCTURAL,
            rootNode
        );
        k20Nodes[machine_i] = machine;
        hwa.addInterconnectBidirectional(machine, switch2, "IB");
        std::vector<dodo::utility::TreeID> numaNodes(nSockets);
        for(unsigned socket_i=0; socket_i<nSockets ; ++socket_i)
        {
            auto numa = hwa.addNode(
                "NUMA-Node" + std::to_string( socket_i ),
                dodo::hardware::NodeType::MEMORY,
                machine
            );
            hwa.setCapacity(numa, 33554432);
            numaNodes[socket_i] = numa;
        }

        for(unsigned socket_i=0; socket_i<nSockets ; ++socket_i)
        {
            for(unsigned socket_j = socket_i+1; socket_j<nSockets ; ++socket_j)
            {
                hwa.addInterconnectBidirectional(numaNodes[socket_i], numaNodes[socket_j], "QPI");
            }
        }


        for(unsigned socket_i=0; socket_i<nSockets ; ++socket_i)
        {
            dodo::utility::TreeID numa = numaNodes[socket_i];
            hwa.addInterconnectBidirectional(numa, machine, "PCI");
            auto package = hwa.addNode(
                "Intel(R) Xeon(R) CPU E5-2609 0 @ 2.40GHz",
                dodo::hardware::NodeType::STRUCTURAL,
                numa
            );
            auto l3 = hwa.addNode(
                "L3",
                dodo::hardware::NodeType::CACHE,
                package
            );
            hwa.setCapacity(l3, 10240);
            hwa.addInterconnectBidirectional(numa, l3, "FSB");
            for(auto numa_other : numaNodes)
            {
                hwa.addToMemHierarchy(l3, numa_other);
            }

            for(unsigned core_i=0; core_i<nCores; ++core_i)
            {
                auto l2 = hwa.addNode( "L2", dodo::hardware::NodeType::CACHE, l3 );
                hwa.setCapacity(l2, 256);
                auto l1 = hwa.addNode( "L1", dodo::hardware::NodeType::CACHE, l2 );
                hwa.setCapacity(l1, 32);
                auto core = hwa.addNode( "Core"+ std::to_string(core_i), dodo::hardware::NodeType::COMPUTE, l1 );
                hwa.gigaFLOPSMap.insert( std::make_pair( core, 153.6 ) );
                hwa.addToMemHierarchy(core, l1);
                hwa.addToMemHierarchy(l1, l2);
                hwa.addToMemHierarchy(l2, l3);
                hwa.addInterconnectBidirectional(l3, l2, "L3L2");
                hwa.addInterconnectBidirectional(l2, l1, "L2L1");
                hwa.addInterconnectBidirectional(l1, core, "CoreL1");
            }
        }
    }

    // See https://en.wikipedia.org/wiki/List_of_device_bit_rates
    std::map<std::string, size_t> nameBandwidthMap;
    nameBandwidthMap["PCI"] = 4*5000u; //quad-lane PCI-E 2.0 in MBit/s
    nameBandwidthMap["IB"] = 9700u;    //single link IB FDR-10 in MBit/s
    nameBandwidthMap["QPI"] = 153600u;      //2.4 GHz as from the processor
    // See http://www.7-cpu.com/cpu/Haswell.html
    nameBandwidthMap["L2L1"] = 2400u * 1000 * 1000 * 64 * 8 / 2.3; //2.4 GHz, theoretical peak of 64bytes per 2.3 cycles
    nameBandwidthMap["L3L2"] = 2400u * 1000 * 1000 * 64 * 8 / 5 - nameBandwidthMap["L2L1"];
    nameBandwidthMap["CoreL1"] = 2400u * 1000 * 1000 * 64 * 8 *2;
    nameBandwidthMap["FSB"] = 14500u * 8; //MBit/s


    auto allCableIter = hwa.ig.getEdges();
    for(auto i(allCableIter.first); i!=allCableIter.second; ++i)
    {
        const std::string cableName = hwa.cableNameMap[(*i)];
        for(auto possibleName : nameBandwidthMap)
        {
            if(cableName == possibleName.first)
            {
                hwa.bandwidthMap.insert(std::make_pair(*i, dodo::hardware::Bandwidth(possibleName.second)));
            }
        }
    }


    dodo::graph::TreeIDGraph threadGraph;
    std::map<dodo::graph::TreeIDGraph::VertexID, dodo::graph::TreeIDGraph::TreeID > threadMapping;

    auto l3Caches = hwa.filterVertices(
        [ &hwa ]( auto i )
        {
            if( hwa.nameMap[i].find( "NUMA-Node" ) != std::string::npos )
            {
                return true;
            }
            return false;
        }
    );


    std::map<dodo::graph::TreeIDGraph::VertexID, ThreadGraphNodeType> threadGraphTypeMap;

    for(int i=0; i<8; ++i){
        auto processVertex = threadGraph.addVertex();
        threadGraphTypeMap[processVertex] = ThreadGraphNodeType::PROCESS;
        threadMapping[processVertex] = l3Caches[i];
        auto cores = hwa.getLeafChildren(l3Caches[i]);
        for(int j=0; j<4 ; ++j)
        {
            auto v = threadGraph.addVertex();
            threadGraphTypeMap[v] = ThreadGraphNodeType::THREAD;
            threadGraph.addEdge(processVertex, v);
            threadMapping[v] = cores.at(j);
        }
    }

    for(auto t : threadMapping)
    {
        std::cout << "ThreadGraph ID " << t.first << "   -->   HardwareGraph ID " << t.second << std::endl;
        if(threadGraphTypeMap[t.first] == ThreadGraphNodeType::THREAD)
        {
            std::cout << "Speed: " << hwa.gigaFLOPSMap.at(t.second) << " GFLOPS" << std::endl;
            auto mems = hwa.getBackingMemories(t.second);
            std::cout << "Availabe Memories:";
            for(auto m : mems)
            {
                std::cout << " " << hwa.nameMap.at(m) << " (" << hwa.capacityMap.at(m).getFreeCapacity() << " KByte)";
            }
            std::cout << std::endl;
        }
    }

    hwa.writeAllTreeIDGraphs("/tmp/");
    hwa.writeAllMaps("/tmp/");


    return 0;
}


