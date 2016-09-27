#include<mpi.h>

#include <iostream>
#include <vector>
#include <cstdlib>

void print(int,int,MPI_Comm &);
void updateAlive(int&, std::array<int,4>&, MPI_Comm &);
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
        MPI_Send( &alive, 1, MPI_INT, 0, 1, comm );
    }
    else
    {
        std::array< int, 16 > alive_states;
        alive_states[0] = alive;
        MPI_Status status;
        for( size_t i = 1; i < 16; ++i )
        {
            MPI_Recv( &alive_states[i], 1, MPI_INT, static_cast<int>(i), 1, comm, &status );
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
    std::array<int,4> & neighborRanks,
    MPI_Comm & comm
)
{
    for( auto r : neighborRanks )
    {
        MPI_Send( &alive, 1, MPI_INT, r, 0, comm );
    }
    int aliveN = 0;
    for( auto r : neighborRanks )
    {
        (void)r;
        int rX=0;
        MPI_Status status;
        MPI_Recv( &rX, 1, MPI_INT, r, 0, comm, &status );
        if(status.MPI_ERROR != 0) throw std::runtime_error("error");
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
    MPI_Cart_rank( comm, neighborCoord.data( ), &id );
    neighborRanks[0] = id;

    //East
    neighborCoord = { { ( coord[0] + 1 ) % 4, coord[1] } };
    MPI_Cart_rank( comm, neighborCoord.data( ), &id );
    neighborRanks[1] = id;

    //South
    neighborCoord = { { coord[0], ( coord[1] + 1 ) % 4 } };
    MPI_Cart_rank( comm, neighborCoord.data( ), &id );
    neighborRanks[2] = id;

    //West
    neighborCoord = { { ( coord[0] + 3 ) % 4, coord[1] } };
    MPI_Cart_rank( comm, neighborCoord.data( ), &id );
    neighborRanks[3] = id;
}


/* A two-dimensional torus of 16 processes on a square grid */
int main(
    int argc,
    char *argv[]
)
{
    int rank = -1;
    int nRanks = 0;
    std::array< int, 2 > dim( { { 4, 4 } } );
    std::array< int, 2 > boundaryCondition( { { 1, 1 } } );
    std::array< int, 2 > coord;
    std::array< int, 4 > neighborRanks;
    int alive;

    // initialization
    MPI_Comm comm;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &nRanks );
    if( nRanks != 16 )
    {
        std::cerr << "Use 16 MPI Processes!" << std::endl;
        MPI_Abort( MPI_COMM_WORLD, 1 );
    }

    MPI_Cart_create( MPI_COMM_WORLD, 2, dim.data( ), boundaryCondition.data( ), 1, &comm );
    MPI_Cart_coords( comm, rank, 2, coord.data( ) );
    std::srand( static_cast< unsigned >( rank ) );
    alive = std::rand( ) % 2;
    findNeighbors( coord, neighborRanks, comm );

//    std::cout << rank;
//    for(auto r : neighborRanks)
//    {
//        std::cout << " " << r;
//    }
//    std::cout << std::endl;


    // Start simulation

    for( int timestep = 0; timestep < 10; ++timestep )
    {
        print( rank, alive, comm );
        updateAlive( alive, neighborRanks, comm );
    }

    MPI_Finalize( );
    return 0;
}


