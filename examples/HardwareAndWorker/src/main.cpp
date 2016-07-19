#include <iostream>
#include <memory>

#include <dodo2.hpp>


int main( )
{

    using HardwareAbstraction = dodo::model::hardware::HardwareAbstraction<
        dodo::model::hardware::extension::MemoryUsage,
        dodo::model::hardware::extension::InterconnectBandwidth,
        dodo::model::hardware::extension::VertexSpeed
    >;

//    HardwareAbstraction hwa;
    auto hwa = std::make_shared<HardwareAbstraction>();

    auto rootNode = hwa->addRoot("Hypnos", dodo::model::hardware::property::VertexType::STRUCTURAL);
    auto switch1 = hwa->add("IB-Switch", dodo::model::hardware::property::VertexType::INTERCONNECT, rootNode);

    constexpr unsigned nMachines = 1;
    constexpr unsigned nSockets = 1;
    constexpr unsigned nCores = 4;

    std::vector<dodo::utility::TreeID> k20Nodes(nMachines);
    for(unsigned machine_i = 0; machine_i < nMachines; ++machine_i)
    {
        auto machine = hwa->add(
            "KeplerNode",
            dodo::model::hardware::property::VertexType::STRUCTURAL,
            rootNode
        );
        k20Nodes[machine_i] = machine;
        hwa->addInterconnectBidirectional(machine, switch1, "IB");
        std::vector<dodo::utility::TreeID> numaNodes(nSockets);
        for(unsigned socket_i=0; socket_i<nSockets ; ++socket_i)
        {
            auto numa = hwa->add(
                "NUMA-Node",
                dodo::model::hardware::property::VertexType::MEMORY,
                machine
            );
            hwa->setCapacity( numa, 33554432 );
            numaNodes[socket_i] = numa;
        }

        for(unsigned socket_i=0; socket_i<nSockets ; ++socket_i)
        {
            for(unsigned socket_j = socket_i+1; socket_j<nSockets ; ++socket_j)
            {
                hwa->addInterconnectBidirectional(numaNodes[socket_i], numaNodes[socket_j], "QPI");
            }
        }


        for(unsigned socket_i=0; socket_i<nSockets ; ++socket_i)
        {
            dodo::utility::TreeID numa = numaNodes[socket_i];
            hwa->addInterconnectBidirectional(numa, machine, "PCI");
            auto package = hwa->add(
                "Intel(R) Xeon(R) CPU E5-2609 0 @ 2.40GHz",
                dodo::model::hardware::property::VertexType::STRUCTURAL,
                numa
            );
            auto l3 = hwa->add(
                "L3",
                dodo::model::hardware::property::VertexType::CACHE,
                package
            );
            hwa->setCapacity(l3, 10240);
            hwa->addInterconnectBidirectional(numa, l3, "FSB");
            for(auto numa_other : numaNodes)
            {
                hwa->addToMemHierarchy(l3, numa_other);
            }

            for(unsigned core_i=0; core_i<nCores; ++core_i)
            {
                auto l2 = hwa->add( "L2", dodo::model::hardware::property::VertexType::CACHE, l3 );
                hwa->setCapacity(l2, 256);
                auto l1 = hwa->add( "L1", dodo::model::hardware::property::VertexType::CACHE, l2 );
                hwa->setCapacity(l1, 32);
                auto core = hwa->add( "Core", dodo::model::hardware::property::VertexType::COMPUTE, l1 );
//                hwa->gigaFLOPSMap.insert( std::make_pair( core, 153.6 ) );
                hwa->addToMemHierarchy(core, l1);
                hwa->addToMemHierarchy(l1, l2);
                hwa->addToMemHierarchy(l2, l3);
                hwa->addInterconnectBidirectional(l3, l2, "L3L2");
                hwa->addInterconnectBidirectional(l2, l1, "L2L1");
                hwa->addInterconnectBidirectional(l1, core, "CoreL1");
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

    auto allCableIter = hwa->getAllInterconnects();
    for(auto i(allCableIter.first); i!=allCableIter.second; ++i)
    {
        const std::string cableName = hwa->getProperty<std::string>("EdgeName", *i);
        for(auto possibleName : nameBandwidthMap)
        {
            if(cableName == possibleName.first)
            {
                hwa->setProperty("InterconnectBandwidth", *i, possibleName.second);
            }
        }
    }

    // **************************
    // * end of defining HWA
    // **************************

//    auto workerModel = std::make_shared<dodo::model::worker::Model>(
//        nMachines,
//        nCores
//    );

    auto workerModel = std::make_shared<dodo::model::worker::Model>( );

    /**
     * definie the Mapping
     */

    using namespace dodo::model::hardware;

//    std::map< HardwareAbstraction::HardwareID , std::vector<dodo::model::worker::Model::WorkerID> > hw2workerMap;
//    auto aspaces = workerModel->getAllAddressSpaces( ).first;
//    for( auto & machine : hwa->getHWElementsByName("KeplerNode"))
//    {
//        auto aspace = *aspaces;
//        std::advance(aspaces, 1);
//        hw2workerMap[machine].push_back(aspace);
//        auto workers = workerModel->getWorkersInAddressSpace( aspace ).first;
//        for( auto & core : hwa->getAllChildrenWithType( k20Nodes[0], property::COMPUTE ) )
//        {
//            hw2workerMap[core].push_back(*workers);
//            std::advance(workers,1);
//        }
//    }


    dodo::mapping::worker2hardware::Interface<HardwareAbstraction> worker2hwMapping(workerModel, hwa);

    worker2hwMapping.generateTrivialMapping(1, {"KeplerNode"});




    hwa->writeAllGraphs("/tmp/");




    return 0;
}
