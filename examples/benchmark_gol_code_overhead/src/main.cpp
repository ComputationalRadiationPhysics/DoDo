#include<mpi.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <dodo2.hpp>


void print(
    int,
    int,
    MPI_Comm &
);
void updateAlive(
    int &,
    std::array< int, 4 > &,
    MPI_Comm &
);
void findNeighbors(
    std::array< int, 2 > const &,
    std::array< int, 4 > &,
    MPI_Comm &
);
void
print(
    int rank,
    int alive,
    MPI_Comm & comm
)
{
    if( rank != 0 )
    {
        MPI_Send(
            &alive,
            1,
            MPI_INT,
            0,
            1,
            comm
        );
    }
    else
    {
        std::array< int, 16 > alive_states;
        alive_states[0] = alive;
        MPI_Status status;
        for( size_t i = 1 ; i < 16 ; ++i )
        {
            MPI_Recv(
                &alive_states[i],
                1,
                MPI_INT,
                static_cast< int >( i ),
                1,
                comm,
                &status
            );
        }
        for( size_t i = 0; i < 16; ++i )
        {
            std::cout << alive_states[i];
            if( i % 4 == 3 )
            {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

void
updateAlive(
    int & alive,
    std::array< int, 4 > & neighborRanks,
    MPI_Comm & comm
)
{
    for( auto r : neighborRanks )
    {
        MPI_Send(
            &alive,
            1,
            MPI_INT,
            r,
            0,
            comm
        );
    }
    int aliveN = 0;
    for( auto r : neighborRanks )
    {
        (void)r;
        int rX=0;
        MPI_Status status;
        MPI_Recv(
            &rX,
            1,
            MPI_INT,
            r,
            0,
            comm,
            &status
        );
        if( status.MPI_ERROR != 0 )
        {
            throw std::runtime_error( "error" );
        }
        aliveN += rX;
    }
    if( aliveN == 2 )
    {
    }else if( aliveN == 3 )
    {
        alive = true;
    } else
    {
        alive = false;
    }
}


void
findNeighbors(
    std::array< int, 2 > const & coord,
    std::array< int, 4 > & neighborRanks,
    MPI_Comm & comm
)
{// Find Neighbors
    int id;
    std::array< int, 2 > neighborCoord; //North
    neighborCoord = { { coord[0], ( coord[1] + 3 ) % 4 } };
    MPI_Cart_rank(
        comm,
        neighborCoord.data(),
        &id
    );
    neighborRanks[0] = id;

    //East
    neighborCoord = { { ( coord[0] + 1 ) % 4, coord[1] } };
    MPI_Cart_rank(
        comm,
        neighborCoord.data(),
        &id
    );
    neighborRanks[1] = id;

    //South
    neighborCoord = { { coord[0], ( coord[1] + 1 ) % 4 } };
    MPI_Cart_rank(
        comm,
        neighborCoord.data(),
        &id
    );
    neighborRanks[2] = id;

    //West
    neighborCoord = { { ( coord[0] + 3 ) % 4, coord[1] } };
    MPI_Cart_rank(
        comm,
        neighborCoord.data(),
        &id
    );
    neighborRanks[3] = id;
}


// A two-dimensional torus of 16 processes on a square grid
int main(
    int argc,
    char *argv[]
)
{
    //typedef//xx
    using MyHWAbs = dodo::model::hardware::HardwareAbstraction< //xx
        dodo::model::hardware::extension::VertexSpeed,//xx
        dodo::model::hardware::extension::InterconnectBandwidth//xx
    >;//xx
    auto hwa = std::make_shared<MyHWAbs>();//xx

    // Hardware architecture//xx
    auto rootNode = hwa->addRoot(//xx
        "Hypnos",//xx
        dodo::model::hardware::property::VertexType::STRUCTURAL//xx
    );//xx
    std::vector<MyHWAbs::HardwareID> computeNodes(n);//xx
    {//xx
        const boost::array< std::size_t, 2 > lengths = { { 2,2 } };//xx
        const boost::grid_graph< 2 > grid(//xx
            lengths,//xx
            { { true, true} }//xx
        );//xx
        for( size_t i=0; i<n ; ++i)//xx
        {//xx
            // each compute node gets 4 cores//xx
            computeNodes[i] = hwa->add(//xx
                "CompNode",//xx
                dodo::model::hardware::property::VertexType::MACHINE,//xx
                rootNode//xx
            );//xx
            for( int j = 0 ; j < 4 ; ++j )//xx
            {//xx
                auto core = hwa->add(//xx
                    "Core",//xx
                    dodo::model::hardware::property::VertexType::COMPUTE,//xx
                    computeNodes[i]//xx
                );//xx
                hwa->setProperty(//xx
                    "VertexSpeed",//xx
                    core,//xx
                    2400//xx
                );//xx
                auto edge = hwa->addInterconnectBidirectional(//xx
                    computeNodes[i],//xx
                    core,//xx
                    "PCI"//xx
                );//xx
                hwa->setProperty(//xx
                    "InterconnectBandwidth",//xx
                    edge,//xx
                    100000//xx
                );//xx
            }//xx
        }//xx
        for( decltype( n ) vid = 0 ; vid < n ; ++vid )//xx
        {//xx
            // compute nodes are connected through a 2D torus network//xx
            const auto treeID = computeNodes[vid];//xx
            const auto gridVertex = vertex(//xx
                vid,//xx
                grid//xx
            );//xx
            for( auto & e : boost::make_iterator_range( out_edges(//xx
                            gridVertex,//xx
                            grid//xx
                        ) ) )//xx
            {//xx
                const auto toVertexInGrid = get(//xx
                    boost::vertex_index,//xx
                    grid,//xx
                    e.second//xx
                );//xx
                auto const toTreeID = computeNodes[ toVertexInGrid ];//xx
                auto const edge = hwa->addInterconnect(//xx
                    treeID,//xx
                    toTreeID,//xx
                    "Infiniband"//xx
                );//xx
                hwa->setProperty(//xx
                    "InterconnectBandwidth",//xx
                    edge,//xx
                    1000//xx
                );//xx
            }//xx
        }//xx
    }//xx
    int rank = -1;
    int nRanks = 0;
    std::array< int, 2 > dim( { { 4, 4 } } );
    std::array< int, 2 > boundaryCondition( { { 1, 1 } } );
    std::array< int, 2 > coord;
    std::array< int, 4 > neighborRanks;
    int alive;

    // initialization
    MPI_Comm comm;
    MPI_Init(
        &argc,
        &argv
    );
    MPI_Comm_rank(
        MPI_COMM_WORLD,
        &rank
    );
    MPI_Comm_size(
        MPI_COMM_WORLD,
        &nRanks
    );
    if( nRanks != 16 )
    {
        std::cerr << "Use 16 MPI Processes!" << std::endl;
        MPI_Abort(
            MPI_COMM_WORLD,
            1
        );
    }


    MPI_Cart_create(
        MPI_COMM_WORLD,
        2,
        dim.data( ),
        boundaryCondition.data( ),
        1,
        &comm
    );

    // Model the MPI ranks as workers.//xx
    // 4 Workers on each node (one per Core)//xx
    auto workerModel = std::make_shared< dodo::model::worker::Model >( );//xx
    dodo::mapping::worker2hardware::Interface< MyHWAbs > worker2hwMapping(//xx
        workerModel,//xx
        hwa//xx
    );//xx
    worker2hwMapping.generateTrivialMapping(//xx
        1,//xx
        { "CompNode" }//xx
    );//xx
    assert(//xx
        dodo::mapping::worker2hardware::checkMemoryLegality( worker2hwMapping )//xx
    );//xx

    MPI_Cart_coords(
        comm,
        rank,
        2,
        coord.data( )
    );
    findNeighbors(
        coord,
        neighborRanks,
        comm
    );

    std::srand( static_cast< unsigned >( rank ) );
    alive = std::rand( ) % 2;

    auto physDom = dodo::model::data::WrappedGrid2D::generate(//xx
        4,//xx
        4//xx
    );//xx
    auto dataAbstraction = std::make_shared<//xx
        dodo::model::data::Abstraction<dodo::model::data::WrappedGrid3D>//xx
    >( physDom );//xx
    dodo::model::data::DataDomain livelinessStates;//xx
    for(auto & v : boost::make_iterator_range( physDom.g.getVertices( ) ) )//xx
    {//xx
        auto d = livelinessStates.createDataAtPos(v);//xx
        livelinessStates.setProperty(//xx
            "sizeInKB",//xx
            d,//xx
            0.004f//xx
        );//xx
    }//xx
    dataAbstraction->addDataDomain(//xx
        std::move( livelinessStates ),//xx
        "livelinessStates"//xx
    );//xx
    auto stateMap = dodo::mapping::data2worker::generateRoundRobin(//xx
        livelinessStates,//xx
        *workerModel//xx
    );//xx
    std::map< std::string, decltype(stateMap) > finalMap;//xx
    finalMap["livelinessStates"] = stateMap;//xx
    dodo::mapping::data2worker::Interface<//xx
        dodo::model::data::WrappedGrid2D//xx
    > data2workerMapping(//xx
        dataAbstraction,//xx
        workerModel,//xx
        finalMap//xx
    );//xx

    // Start simulation
    for( int timestep = 0 ; timestep < 10 ; ++timestep )
    {
        print(
            rank,
            alive,
            comm
        );
        updateAlive(
            alive,
            neighborRanks,
            comm
        );
    }
    MPI_Finalize( );
    return 0;
}
