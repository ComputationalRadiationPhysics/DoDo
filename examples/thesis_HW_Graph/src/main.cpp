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

    auto rootNode = hwa.addRoot("Cluster", dodo::hardware::NodeType::STRUCTURAL);
    auto switch1 = hwa.addNode("Switch", dodo::hardware::NodeType::INTERCONNECT, rootNode);

    constexpr unsigned nMachines = 2;
    constexpr unsigned nSockets = 1;
    constexpr unsigned nCores = 1;
    constexpr unsigned nGPUs = 0;
    constexpr unsigned nSMs = 0;

    std::vector<dodo::utility::TreeID> k20Nodes(nMachines);
    for(unsigned machine_i = 0; machine_i < nMachines; ++machine_i)
    {
        auto machine = hwa.addNode(
            "ComputeNode",
            dodo::hardware::NodeType::STRUCTURAL,
            rootNode
        );
        k20Nodes[machine_i] = machine;
        std::vector<dodo::utility::TreeID> numaNodes(nSockets);
        for(unsigned socket_i=0; socket_i<nSockets ; ++socket_i)
        {
            auto numa = hwa.addNode(
                "RAM",
                dodo::hardware::NodeType::MEMORY,
                machine
            );
            hwa.setCapacity(numa, 33554432);
            numaNodes[socket_i] = numa;
        }

        for(unsigned gpu_i=0; gpu_i<nGPUs ; ++gpu_i)
        {
            auto gpu = hwa.addNode(
                "nVidia K20m",
                dodo::hardware::NodeType::STRUCTURAL,
                machine
            );
            auto globalMem = hwa.addNode(
                "globalMem",
                dodo::hardware::NodeType::MEMORY,
                gpu
            );
            hwa.setCapacity(globalMem, 5242880);
            auto l2 = hwa.addNode(
                "L2_GPU",
                dodo::hardware::NodeType::CACHE,
                globalMem
            );
            hwa.addInterconnectBidirectional(globalMem, l2, "CUDA_L2_GLOBAL");
            hwa.setCapacity(l2, 1280);
            hwa.addToMemHierarchy(l2, globalMem);

            for(unsigned sm_i=0; sm_i<nSMs ; ++sm_i)
            {
                auto l1 = hwa.addNode(
                    "L1_SM",
                    dodo::hardware::NodeType::CACHE,
                    l2
                );

                auto sm = hwa.addNode(
                    "SM",
                    dodo::hardware::NodeType::COMPUTE,
                    globalMem
                );

                hwa.setCapacity(l1, 48);
                hwa.addInterconnectBidirectional(l2, l1, "CUDA_L2_L1");
                auto sharedMem = hwa.addNode(
                    "sharedMem",
                    dodo::hardware::NodeType::MEMORY,
                    l2
                );
                hwa.setCapacity(sharedMem, 16);
                hwa.addInterconnectBidirectional(l2, sharedMem, "CUDA_L2_L1");
                hwa.addToMemHierarchy(l1, l2);
                hwa.addToMemHierarchy(sm, l1);
                hwa.addToMemHierarchy(sm, sharedMem);
                hwa.addInterconnectBidirectional(sm, l1, "CUDA_SM_L1");
                hwa.addInterconnectBidirectional(sm, sharedMem, "CUDA_SM_L1");

            }
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
                "CPU",
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
                auto core = hwa.addNode( "Core", dodo::hardware::NodeType::COMPUTE, l1 );
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
    nameBandwidthMap["CUDA_L2_GLOBAL"] = 208 * 1024 * 8; // MBit/s
    nameBandwidthMap["CUDA_L2_L1"] = 100 * 1024 * 8; // MBit/s
    nameBandwidthMap["CUDA_SM_L1"] = 64u * 706u * 1000u * 1000u; // MBit/s


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


    hwa.writeAllTreeIDGraphs("/tmp/");

    return 0;
}


