#include <mpi.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <dodo.hpp>

template<typename T_DataModel>//x4x
struct GOLRule : dodo::model::routine::ComponentBase<T_DataModel> {//x4x
  virtual float effort(std::shared_ptr<T_DataModel> dataModel) override//x4x
  {//x4x
      (void) dataModel;//x4x
      return 10;//x4x
  }//x4x
};//x4x

template<typename T_DataModel>//x4x
struct GOLPrint : dodo::model::routine::ComponentBase<T_DataModel> {//x4x
  virtual float effort(std::shared_ptr<T_DataModel> dataModel) override//x4x
  {//x4x
      return dataModel->template getProperty<float>(this->inData[0],//x4x
              "sizeInKB")*8;//x4x
  }//x4x
};//x4x

void print(int, int, MPI_Comm&);

void updateAlive(int&, std::array<int, 4>&, MPI_Comm&);

void findNeighbors(std::array<int, 2> const&,  std::array<int, 4>&,  MPI_Comm&);

void
print(int rank, int alive, MPI_Comm& comm)
{
    if (rank!=0) {
        MPI_Send(&alive, 1, MPI_INT, 0, 1, comm);
    }
    else {
        std::array<int, 16> alive_states;
        alive_states[0] = alive;
        MPI_Status status;
        for (size_t i = 1; i<16; ++i) {
            MPI_Recv(&alive_states[i], 1, MPI_INT, static_cast< int >( i ), 1,
                    comm, &status);
        }
        for (size_t i = 0; i<16; ++i) {
            std::cout << alive_states[i];
            if (i%4==3) {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

void updateAlive(int& alive, std::array<int, 4>& neighborRanks, MPI_Comm& comm)
{
    for (auto r : neighborRanks) {
        MPI_Send(&alive, 1, MPI_INT, r, 0, comm);
    }
    int aliveN = 0;
    for (auto r : neighborRanks) {
        (void) r;
        int rX = 0;
        MPI_Status status;
        MPI_Recv(&rX, 1, MPI_INT, r, 0, comm, &status);
        if (status.MPI_ERROR!=0) {
            throw std::runtime_error("error");
        }
        aliveN += rX;
    }
    if (aliveN==2) {
    }
    else if (aliveN==3) {
        alive = true;
    }
    else {
        alive = false;
    }
}

void findNeighbors(std::array<int, 2> const& coord,
        std::array<int, 4>& neighborRanks, MPI_Comm& comm)
{// Find Neighbors
    int id;
    std::array<int, 2> neighborCoord; //North
    neighborCoord = {{coord[0], (coord[1]+3)%4}};
    MPI_Cart_rank(comm, neighborCoord.data(), &id);
    neighborRanks[0] = id;

    //East
    neighborCoord = {{(coord[0]+1)%4, coord[1]}};
    MPI_Cart_rank(comm, neighborCoord.data(), &id);
    neighborRanks[1] = id;

    //South
    neighborCoord = {{coord[0], (coord[1]+1)%4}};
    MPI_Cart_rank(comm, neighborCoord.data(), &id);
    neighborRanks[2] = id;

    //West
    neighborCoord = {{(coord[0]+3)%4, coord[1]}};
    MPI_Cart_rank(comm, neighborCoord.data(), &id);
    neighborRanks[3] = id;
}

// A two-dimensional torus of 16 processes on a square grid
int main(int argc, char* argv[])
{
    //typedef//x1x
    namespace hwmodel = dodo::model::hardware;//x1x
    using hwVertex = dodo::model::hardware::property::VertexType;//x1x
    using MyHWAbs = hwmodel::HardwareAbstraction<//x1x
            hwmodel::extension::VertexSpeed,//x1x
            hwmodel::extension::InterconnectBandwidth>;//x1x
    auto hwa = std::make_shared<MyHWAbs>();//x1x

    // Hardware architecture//x1x
    auto rootNode = hwa->addRoot("Hypnos", hwVertex::STRUCTURAL);//x1x
    std::vector<MyHWAbs::HardwareID> computeNodes(4);//x1x
    {//x1x
        const boost::array<std::size_t, 2> lengths = {{2, 2}};//x1x
        const boost::grid_graph<2> grid(lengths, {{true, true}});//x1x
        for (size_t i = 0; i<computeNodes.size(); ++i)//x1x
        {//x1x
            // each compute node gets 4 cores//x1x
            computeNodes[i] = hwa->add("CompNode", hwVertex::MACHINE, rootNode);//x1x
            auto memory = hwa->add("RAM", hwVertex::MEMORY, computeNodes[i]);//x1x
            auto edge = hwa->addInterconnectBidirectional(memory,//x1x
                    computeNodes[i], "PCIe");//x1x
            for (int j = 0; j<4; ++j)//x1x
            {//x1x
                auto core = hwa->add("Core", hwVertex::COMPUTE, computeNodes[i]);//x1x
                hwa->setProperty("VertexSpeed", core, std::size_t(2400));//x1x
                auto mEdge = hwa->addInterconnectBidirectional(memory, core,//x1x
                        "FSB");//x1x
                hwa->setProperty("InterconnectBandwidth", mEdge,//x1x
                        std::size_t(100000));//x1x
                hwa->addToMemHierarchy(core, memory);//x1x
            }//x1x
        }//x1x
        for (unsigned vid = 0; vid<computeNodes.size(); ++vid)//x1x
        {//x1x
            // compute nodes are connected through a 2D torus network//x1x
            const auto treeID = computeNodes[vid];//x1x
            const auto gridVertex = vertex(vid, grid);//x1x
            for (auto e : boost::make_iterator_range(//x1x
                    out_edges(gridVertex, grid)))//x1x
            {//x1x
                auto toVertexInGrid = get(boost::vertex_index, grid, e.second);//x1x
                auto const toTreeID = computeNodes[toVertexInGrid];//x1x
                auto edge = hwa->addInterconnect(treeID, toTreeID, "Infiniband");//x1x
                hwa->setProperty(std::string("InterconnectBandwidth"), edge,//x1x
                        std::size_t(1000));//x1x
            }//x1x
        }//x1x
    }//x1x
    int rank = -1;
    int nRanks = 0;
    std::array<int, 2> dim({{4, 4}});
    std::array<int, 2> boundaryCondition({{1, 1}});
    std::array<int, 2> coord;
    std::array<int, 4> neighborRanks;
    int alive;

    // initialization
    MPI_Comm comm;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);
    if (nRanks!=16) {
        std::cerr << "Use 16 MPI Processes!" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Cart_create(MPI_COMM_WORLD, 2, dim.data(), boundaryCondition.data(), 1,
            &comm);

    // Model the MPI ranks as workers.//x2x
    // 4 Workers on each node (one per Core)//x2x
    namespace worker = dodo::model::worker;//x2x
    namespace workerMapping = dodo::mapping::worker2hardware;//x2x
    auto workerModel = std::make_shared<worker::Model>();//x2x
    workerMapping::Interface<MyHWAbs> worker2hwMapping(workerModel, hwa);//x2x
    worker2hwMapping.generateTrivialMapping(1, {"CompNode"});//x2x
    assert(workerMapping::checkMemoryLegality(worker2hwMapping));//x2x

    MPI_Cart_coords(comm, rank, 2, coord.data());
    findNeighbors(coord, neighborRanks, comm);

    std::srand(static_cast< unsigned >( rank ));
    alive = std::rand()%2;

    namespace data = dodo::model::data;//x3x
    using PhysicalDomain = data::WrappedGrid2D;//x3x
    auto physDom = PhysicalDomain::generate(4, 4);//x3x
    using DataAbstraction = data::Abstraction<PhysicalDomain>;//x3x
    auto dataAbstraction = std::make_shared<DataAbstraction>(physDom);//x3x
    data::DataDomain livelinessStates;//x3x
    for (auto v : boost::make_iterator_range(physDom.g.getVertices()))//x3x
    {//x3x
        auto d = livelinessStates.createDataAtPos(v);//x3x
        livelinessStates.setProperty("sizeInKB", d, 0.004f);//x3x
    }//x3x
    dataAbstraction->addDataDomain(std::move(livelinessStates), //x3x
            "livelinessStates");//x3x
    auto stateMap = dodo::mapping::data2worker::generateRoundRobin(//x3x
            livelinessStates, *workerModel);//x3x
    std::map<std::string, decltype(stateMap)> finalMap;//x3x
    finalMap["livelinessStates"] = stateMap;//x3x
    dodo::mapping::data2worker::Interface<PhysicalDomain> data2workerMapping(//x3x
            dataAbstraction, workerModel, finalMap);//x3x

    namespace routine = dodo::model::routine;//x4x
    using Dir = data::traits::Directions<data::WrappedGrid2D>::Values;//x4x
    using PortType = routine::Port<Dir>;//x4x
    routine::ComponentTemplate<GOLRule<DataAbstraction> > golRuleStencil;//x4x
    golRuleStencil.name = "livelinessRule";//x4x
    golRuleStencil.outPorts.push_back(PortType("livelinessStates", Dir::SELF));//x4x

    for (Dir d : {Dir::NORTH, Dir::EAST, Dir::SOUTH, Dir::WEST})//x4x
    {//x4x
        golRuleStencil.inPorts.push_back(PortType("livelinessStates", d));//x4x
    }//x4x

    routine::ComponentTemplate<GOLPrint<DataAbstraction> > golPrintStencil;//x4x
    golPrintStencil.name = "print";//x4x
    golPrintStencil.predecessors.insert(golRuleStencil.name);//x4x
    golPrintStencil.inPorts.push_back(PortType("livelinessStates", Dir::SELF));//x4x

    using RoutineModel = routine::Abstraction<PhysicalDomain>;//x4x
    auto routineModel = std::make_shared<RoutineModel>(dataAbstraction);//x4x
    routineModel->instantiateComponents(golRuleStencil, golPrintStencil);//x4x

    auto componentMap = dodo::mapping::component2worker::generateRoundRobin(//x4x
            routineModel, workerModel);//x4x

    dodo::mapping::component2worker::Interface<PhysicalDomain> componentMapping(//x4x
            routineModel, workerModel, componentMap);//x4x

    // Start simulation
    for (int timestep = 0; timestep<10; ++timestep) {
        updateAlive(alive, neighborRanks, comm);
        print(rank, alive, comm);
    }
    MPI_Finalize();
    return 0;
}
