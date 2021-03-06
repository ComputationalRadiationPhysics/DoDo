#include <iostream>

#include <dodo.hpp>


int main( )
{

    using HardwareAbstraction = dodo::model::hardware::HardwareAbstraction<
        dodo::model::hardware::extension::MemoryUsage,
        dodo::model::hardware::extension::InterconnectBandwidth,
        dodo::model::hardware::extension::VertexSpeed
    >;

    HardwareAbstraction hwa;

    auto rootNode = hwa.addRoot("Hypnos", dodo::model::hardware::property::VertexType::STRUCTURAL);
    auto switch1 = hwa.add("IB-Switch1", dodo::model::hardware::property::VertexType::INTERCONNECT, rootNode);
    auto switch2 = hwa.add("IB-Switch2", dodo::model::hardware::property::VertexType::INTERCONNECT, rootNode);
    auto switch3 = hwa.add("IB-Switch3", dodo::model::hardware::property::VertexType::INTERCONNECT, rootNode);
    auto ibcables1 = hwa.addInterconnectBidirectional(switch1, switch2, "IB");
    auto ibcables2 = hwa.addInterconnectBidirectional(switch1, switch3, "IB");

    constexpr unsigned nMachines = 2;
    constexpr unsigned nSockets = 1;
    constexpr unsigned nCores = 4;
    constexpr unsigned nGPUs = 1;
    constexpr unsigned nSMs = 4;
//
    std::vector<dodo::utility::TreeID> k20Nodes(nMachines);
    for(unsigned machine_i = 0; machine_i < nMachines; ++machine_i)
    {
        auto machine = hwa.add(
            "KeplerNode",
            dodo::model::hardware::property::VertexType::STRUCTURAL,
            rootNode
        );
        k20Nodes[machine_i] = machine;
        hwa.addInterconnectBidirectional(machine, switch2, "IB");
        std::vector<dodo::utility::TreeID> numaNodes(nSockets);
        for(unsigned socket_i=0; socket_i<nSockets ; ++socket_i)
        {
            auto numa = hwa.add(
                "NUMA-Node",
                dodo::model::hardware::property::VertexType::MEMORY,
                machine
            );
            hwa.setProperty("MemoryUsage", numa, 33554432lu);
            numaNodes[socket_i] = numa;
        }

        for(unsigned gpu_i=0; gpu_i<nGPUs ; ++gpu_i)
        {

            auto gpu = hwa.add(
                "nVidia K20m",
                dodo::model::hardware::property::VertexType::STRUCTURAL,
                machine
            );
            auto globalMem = hwa.add(
                "globalMem",
                dodo::model::hardware::property::VertexType::MEMORY,
                gpu
            );
            hwa.addInterconnectBidirectional(globalMem, numaNodes[gpu_i%nSockets], "PCI");
            hwa.setProperty("MemoryUsage", globalMem, 5242880lu);
            auto l2 = hwa.add(
                "L2_GPU",
                dodo::model::hardware::property::VertexType::CACHE,
                globalMem
            );
            hwa.addInterconnectBidirectional(globalMem, l2, "CUDA_L2_GLOBAL");
            hwa.setProperty("MemoryUsage", l2, 1280lu);
            hwa.addToMemHierarchy(l2, globalMem);

            for(unsigned sm_i=0; sm_i<nSMs ; ++sm_i)
            {
                auto l1 = hwa.add(
                    "L1_SM",
                    dodo::model::hardware::property::VertexType::CACHE,
                    l2
                );

                auto sm = hwa.add(
                    "SM",
                    dodo::model::hardware::property::VertexType::COMPUTE,
                    globalMem
                );

                hwa.setCapacity(l1, 48);
                hwa.addInterconnectBidirectional(l2, l1, "CUDA_L2_L1");
                auto sharedMem = hwa.add(
                    "sharedMem",
                    dodo::model::hardware::property::VertexType::MEMORY,
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
            auto package = hwa.add(
                "Intel(R) Xeon(R) CPU E5-2609 0 @ 2.40GHz",
                dodo::model::hardware::property::VertexType::STRUCTURAL,
                numa
            );
            auto l3 = hwa.add(
                "L3",
                dodo::model::hardware::property::VertexType::CACHE,
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
                auto l2 = hwa.add( "L2", dodo::model::hardware::property::VertexType::CACHE, l3 );
                hwa.setCapacity(l2, 256);
                auto l1 = hwa.add( "L1", dodo::model::hardware::property::VertexType::CACHE, l2 );
                hwa.setCapacity(l1, 32);
                auto core = hwa.add( "Core", dodo::model::hardware::property::VertexType::COMPUTE, l1 );
//                hwa.gigaFLOPSMap.insert( std::make_pair( core, 153.6 ) );
                hwa.addToMemHierarchy(core, l1);
                hwa.addToMemHierarchy(l1, l2);
                hwa.addToMemHierarchy(l2, l3);
                hwa.addInterconnectBidirectional(l3, l2, "L3L2");
                hwa.addInterconnectBidirectional(l2, l1, "L2L1");
                hwa.addInterconnectBidirectional(l1, core, "CoreL1");
            }
        }
    }

//    // See https://en.wikipedia.org/wiki/List_of_device_bit_rates
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

    auto allCableIter = hwa.getAllInterconnects();
    for(auto i(allCableIter.first); i!=allCableIter.second; ++i)
    {
        const std::string cableName = hwa.getProperty<std::string>("EdgeName", *i); //hwa.cableNameMap[(*i)];
        for(auto possibleName : nameBandwidthMap)
        {
            if(cableName == possibleName.first)
            {
                hwa.setProperty("InterconnectBandwidth", *i, possibleName.second);
            }
        }
    }


    hwa.writeAllGraphs("/tmp/");

    auto a = hwa.getTransferTime(k20Nodes[0], k20Nodes[1], 1280000000);
    std::cout << "transfer time: " << a << std::endl;


    return 0;
}
