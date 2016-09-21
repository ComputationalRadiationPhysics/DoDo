#include <iostream>
#include <vector>
#include <chrono>
#include <sys/resource.h>

#include <boost/graph/graphml.hpp>
#include <boost/program_options.hpp>

#include <dodo2.hpp>
#include <stdlib.h>


namespace po = boost::program_options;


enum ThreadGraphNodeType{
    PROCESS,
    THREAD,
    _count
};


std::string getMemoryConsumption();
std::string getMemoryConsumption()
{
    struct rusage r_usage;
    getrusage(RUSAGE_SELF,&r_usage);
    return std::to_string( r_usage.ru_maxrss );
}

po::variables_map parseCommandLine(const int, char**);

po::variables_map parseCommandLine(const int argc, char** argv)
{
    po::options_description cmdline_options( "Experiment Options" );
    cmdline_options.add_options( )
    (
        "dim",
        po::value< size_t >( )->default_value( 4 ),
        "the extent in each dimension"
    )
    (
        "help,h",
        "print meaning of output and help"
    );
    po::variables_map vm;
    po::store(po::parse_command_line( argc, argv, cmdline_options ), vm);
    po::notify(vm);

    if(vm.count("help")){
        std::cout << "# extent   nodes   cogvertices   totalProperties    memory    time"<< std::endl;
        exit(0);
    }
    return vm;
}


int main(
    int argc,
    char ** argv
)
{
    auto vm = parseCommandLine( argc, argv );
    std::chrono::time_point<std::chrono::system_clock> start, end;

    using MyHWAbs = dodo::model::hardware::HardwareAbstraction<
        dodo::model::hardware::extension::MemoryUsage,
        dodo::model::hardware::extension::VertexSpeed,
        dodo::model::hardware::extension::InterconnectBandwidth
    >;
    MyHWAbs hwa;

    auto rootNode = hwa.addRoot("Hypnos", dodo::model::hardware::property::VertexType::STRUCTURAL);
    size_t dim = vm["dim"].as<size_t>();
    size_t n = dim*dim*dim;

    std::vector<MyHWAbs::HardwareID> computeNodes(n);
    {
        const boost::array< std::size_t, 3 > lengths = {{ dim, dim, dim }};
        const boost::grid_graph< 3 > grid( lengths, {{false, false, false}} );
        //using GridTraits = boost::graph_traits< boost::grid_graph< 3 > >;
        if( n != num_vertices(grid)){
            std::cerr << "size mismatch!" << std::endl;
            return 1;
        }
        start = std::chrono::system_clock::now();
        for( size_t i=0; i<n ; ++i)
        {
            computeNodes[i] = hwa.add("CompNode"+ std::to_string(i), dodo::model::hardware::property::VertexType::MACHINE, rootNode);
        }

        if(n>1)
        for( decltype(n) vid=0 ; vid< n ; ++vid )
        {
            const auto treeID = computeNodes[vid];
            const auto gridVertex = vertex(vid, grid);
            for(auto e : boost::make_iterator_range(out_edges(gridVertex, grid)))
            {
                const auto toVertexInGrid = get(boost::vertex_index, grid, e.second);
                const auto toTreeID = computeNodes[toVertexInGrid];
                hwa.addInterconnect(treeID, toTreeID, "Infiniband");
            }
        }
    }

    constexpr unsigned nSockets = 1;
    constexpr unsigned nCores = 4;
    constexpr unsigned nGPUs = 1;
    constexpr unsigned nSMs = 4;

    for(auto& machine : computeNodes)
    {
        std::vector<dodo::utility::TreeID> numaNodes(nSockets);
        for(unsigned socket_i=0; socket_i<nSockets ; ++socket_i)
        {
            const auto numa = hwa.add(
                "NUMA-Node",
                dodo::model::hardware::property::VertexType::MEMORY,
                machine
            );
            hwa.setCapacity(numa, 33554432);
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
            hwa.setCapacity(globalMem, 5242880);
            auto l2 = hwa.add(
                "L2_GPU",
                dodo::model::hardware::property::VertexType::CACHE,
                globalMem
            );
            hwa.addInterconnectBidirectional(globalMem, l2, "CUDA_L2_GLOBAL");
            hwa.setCapacity(l2, 1280);
            hwa.addToMemHierarchy(l2, globalMem);

            for(unsigned sm_i=0; sm_i<nSMs ; ++sm_i)
            {
                const auto l1 = hwa.add(
                    "L1_SM",
                    dodo::model::hardware::property::VertexType::CACHE,
                    l2
                );

                const auto sm = hwa.add(
                    "SM",
                    dodo::model::hardware::property::VertexType::COMPUTE,
                    globalMem
                );

                hwa.setCapacity(l1, 48);
                hwa.addInterconnectBidirectional(l2, l1, "CUDA_L2_L1");
                const auto sharedMem = hwa.add(
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
            for(auto& numa_other : numaNodes)
            {
                hwa.addToMemHierarchy(l3, numa_other);
            }

            for(unsigned core_i=0; core_i<nCores; ++core_i)
            {
                auto l2 = hwa.add( "L2", dodo::model::hardware::property::VertexType::CACHE, l3 );
                hwa.setCapacity(l2, 256);
                auto l1 = hwa.add( "L1", dodo::model::hardware::property::VertexType::CACHE, l2 );
                hwa.setCapacity(l1, 32);
                const auto core = hwa.add( "Core", dodo::model::hardware::property::VertexType::COMPUTE, l1 );
                hwa.setProperty("VertexSpeed", core, std::size_t(153) );
                hwa.addToMemHierarchy(core, l1);
                hwa.addToMemHierarchy(l1, l2);
                hwa.addToMemHierarchy(l2, l3);
                hwa.addInterconnectBidirectional(l3, l2, "L3L2");
                hwa.addInterconnectBidirectional(l2, l1, "L2L1");
                hwa.addInterconnectBidirectional(l1, core, "CoreL1");
            }
        }
    }
    computeNodes.clear();

    {
        // See https://en.wikipedia.org/wiki/List_of_device_bit_rates
        std::map<std::string, size_t> nameBandwidthMap;
        nameBandwidthMap["PCI"] = 4*5000u; //quad-lane PCI-E 2.0 in MBit/s
        nameBandwidthMap["IB"] = 9700u;    //single link IB FDR-10 in MBit/s
        nameBandwidthMap["QPI"] = 153600u;      //2.4 GHz as from the processor
        // See http://www.7-cpu.com/cpu/Haswell.html
        nameBandwidthMap["L2L1"] = static_cast<size_t>(2400u * 1000 * 1000 * 64 * 8 / 2.3); //2.4 GHz, theoretical peak of 64bytes per 2.3 cycles
        nameBandwidthMap["L3L2"] = 2400u * 1000 * 1000 * 64 * 8 / 5 - nameBandwidthMap["L2L1"];
        nameBandwidthMap["CoreL1"] = 2400u * 1000 * 1000 * 64 * 8 *2;
        nameBandwidthMap["FSB"] = 14500u * 8; //MBit/s
        nameBandwidthMap["CUDA_L2_GLOBAL"] = 208 * 1024 * 8; // MBit/s
        nameBandwidthMap["CUDA_L2_L1"] = 100 * 1024 * 8; // MBit/s
        nameBandwidthMap["CUDA_SM_L1"] = 64u * 706u * 1000u * 1000u; // MBit/s


        auto allCableIter = hwa.getAllInterconnects();
        for(auto i(allCableIter.first); i!=allCableIter.second; ++i)
        {
            //const std::string cableName = hwa.cableNameMap[(*i)];
            const std::string cableName = hwa.getProperty<std::string>("EdgeName", (*i));
            for(auto possibleName : nameBandwidthMap)
            {
                if(cableName == possibleName.first)
                {
                    hwa.setProperty("InterconnectBandwidth", *i, possibleName.second);
                }
            }
        }
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::string mem = getMemoryConsumption();
    std::cout << dim <<  "    " << n << "    " <<  hwa.getAllChildren(rootNode).size() << "    " << hwa.countProperties() << "    " << mem << "    " << elapsed_seconds.count() << std::endl;
    return 0;
}


