#include <iostream>
#include <boost/graph/graphml.hpp>
#include <boost/graph/grid_graph.hpp>
#include <numeric>
#include <array>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <dodo/graph/SimpleBGL.hpp>




struct VProp
{
    size_t id;
    size_t particleCount;
    unsigned xoffset;
    unsigned yoffset;
    unsigned zoffset;
    unsigned xextent;
    unsigned yextent;
    unsigned zextent;
};

auto genOffsetArray(std::string s)
{
    std::stringstream ss(s);
    boost::array<size_t, 3> a;
    std::string tmp;
    for(int i=0; i<3; ++i)
    {
        std::getline( ss, tmp, ',' );
        a[i] = atoi( tmp.c_str() );
    }
    return a;
}

struct EProp
{
    size_t id;
    std::string outDirection;
};


auto
parsePythonOutput(std::string path, std::string species, unsigned step)
{
    std::ifstream ifs;
    ifs.open(path+ "/" + species + "_" + std::to_string(step) + ".txt", std::ifstream::in);
    std::string line;
    std::map<boost::array<size_t, 3>, std::pair<boost::array<size_t, 3>, size_t>> posMap;

    while(true)
    {
        std::string tmp;
        std::getline(ifs, line);
        if(line == "") break;
        std::stringstream iss(line);

        std::getline(iss, tmp, ' ');
        size_t count = atoi(tmp.c_str());

        std::getline(iss, tmp, ' ');
        auto offsetA = genOffsetArray(tmp);

        std::getline(iss, tmp, '\n');
        auto extentA = genOffsetArray(tmp);
        posMap[offsetA] = std::make_pair(extentA, count);
    }
    return posMap;
}

std::map<boost::array<size_t, 2>, std::pair<boost::array<size_t, 2>, size_t>>
flatten3DParticleMap(
    const std::map<
        boost::array<
            size_t,
            3
        >,
        std::pair<
            boost::array<
                size_t,
                3
            >,
            size_t>> & inputMap
)
{
    std::map<boost::array<size_t, 2>, std::pair<boost::array<size_t, 2>, size_t>> outputMap;
    for(auto& elem : inputMap)
    {
        auto pos = outputMap.find({{elem.first[0], elem.first[1]}});
        if(pos == outputMap.end())
        {
            outputMap[{{elem.first[0], elem.first[1]}}] = std::make_pair(boost::array<size_t, 2>{{elem.second.first[0], elem.second.first[1]}}, elem.second.second);
        }
        else{
           pos->second.second += elem.second.second;
        }
    }
    return outputMap;
}

template<typename T_Map>
auto
applyMapWeights(
    T_Map & inputMap,
    size_t bytesPerParticle
)
{
    for(auto& elem : inputMap)
    {
        elem.second.second *= bytesPerParticle;
    }
    return inputMap;
}

template<typename T>
auto
fieldSizeMap2D(
    const std::map<boost::array<size_t, 3>, std::pair<boost::array<size_t, 3>, T>> & inputMap
)
{
    std::map<boost::array<size_t, 2>, std::pair<boost::array<size_t, 2>, T>> outputMap;
    for(auto& elem : inputMap)
    {
        size_t size = elem.second.first[0]*elem.second.first[1]*elem.second.first[2];
        auto pos = outputMap.find({{elem.first[0], elem.first[1]}});
        if(pos == outputMap.end())
        {
            outputMap[{{elem.first[0], elem.first[1]}}] = std::make_pair(boost::array<size_t, 2>{{elem.second.first[0], elem.second.first[1]}}, size);
        }
        else{
            pos->second.second += size;
        }
    }
    return outputMap;
}

template<typename T>
auto
fieldSizeMap3D(
    const std::map<boost::array<size_t, 3>, std::pair<boost::array<size_t, 3>, T>> & inputMap
)
{
    std::map<boost::array<size_t, 3>, std::pair<boost::array<size_t, 3>, T>> outputMap;
    for(auto& elem : inputMap)
    {
        size_t size = elem.second.first[0]*elem.second.first[1]*elem.second.first[2];
        outputMap[elem.first] = std::make_pair(elem.second.first, size);
    }
    return outputMap;
}

template<typename T>
auto
mapToSet(
    const std::map<
        boost::array<
            size_t,
            2
        >,
        std::pair<
            boost::array<
                size_t,
                2
            >,
            T>> & inputMap
)
{
    std::set<boost::array<size_t, 2>> outputSet;
    for( auto && item : inputMap )
    {
        auto ax = item.first[0];
        auto ay = item.first[1];
        auto extx = ax + item.second.first[0];
        auto exty = ay + item.second.first[1];
        outputSet.insert({{ ax, ay }});
        outputSet.insert({{ extx, ay}});

        outputSet.insert({{ ax, exty }});
        outputSet.insert({{ extx, exty }});
        //        outputSet.insert(std::make_pair(item.first[0], item.first[1]));
        //        outputSet.insert(std::make_pair(item.first[0], item.first[1]+item.second.first[1]));
        //        outputSet.insert(std::make_pair(item.first[0]+item.second.first[0], item.first[1]));
        //        outputSet.insert(std::make_pair(item.first[0]+item.second.first[0], item.first[1]+item.second.first[1]));
    }
    return outputSet;
}

template<typename T>
auto
mapToSet(
    const std::map<
        boost::array<
            size_t,
            3
        >,
        std::pair<
            boost::array<
                size_t,
                3
            >,
            T>> & inputMap
)
{
    std::set<boost::array<size_t, 3>> outputSet;
    for( auto && item : inputMap )
    {
        auto ax = item.first[0];
        auto ay = item.first[1];
        auto az = item.first[2];
        auto extx = ax + item.second.first[0];
        auto exty = ay + item.second.first[1];
        auto extz = az + item.second.first[2];
        outputSet.insert({{ ax, ay, az }});
        outputSet.insert({{ extx, ay, az}});
        outputSet.insert({{ ax, exty, az }});
        outputSet.insert({{ extx, exty, az }});
        outputSet.insert({{ ax, ay, extz }});
        outputSet.insert({{ extx, ay, extz }});
        outputSet.insert({{ ax, exty, extz }});
        outputSet.insert({{ extx, exty, extz }});
    }
    return outputSet;
}


template<typename T>
void writeMapToVTK(
    const std::map<
        boost::array<
            size_t,
            2
        >,
        std::pair<
            boost::array<
                size_t,
                2
            >,
            T>> & flatMap,
    std::string path,
    float scalar = 1.f
)
{
    auto pointSet = mapToSet( flatMap );
    std::ofstream ofs;
    ofs.open(path, std::ios_base::out);
    ofs << "# vtk DataFile Version 2.0" << std::endl;
    ofs << "#my data" << std::endl;
    ofs << "ASCII" << std::endl;
    ofs << "DATASET STRUCTURED_GRID" << std::endl;
    // TODO: give dimensions as parameter
    ofs << "DIMENSIONS " << "21 21 1" << std::endl;
    ofs << "POINTS " << pointSet.size() << " INT" << std::endl;
    for(auto &i : pointSet)
    {
        ofs << i[0] << " " << i[1] << " 0" << std::endl;
    }
    ofs << "CELL_DATA " << flatMap.size() << std::endl;
    ofs << "SCALARS cell_scalars FLOAT 1 " << std::endl;
    ofs << "LOOKUP_TABLE default" << std::endl;
    for(auto &i : flatMap)
    {
        ofs << i.second.second*scalar << std::endl;
    }
}

template<typename T>
void writeMapToVTK(
    const std::map<
        boost::array<
            size_t,
            3
        >,
        std::pair<
            boost::array<
                size_t,
                3
            >,
            T>> & flatMap,
    std::string path,
    float scalar = 1.f
)
{
    auto pointSet = mapToSet( flatMap );
    std::ofstream ofs;
    ofs.open(path, std::ios_base::out);
    ofs << "# vtk DataFile Version 2.0" << std::endl;
    ofs << "#my data" << std::endl;
    ofs << "ASCII" << std::endl;
    ofs << "DATASET STRUCTURED_GRID" << std::endl;
    // TODO: give dimensions as parameter
    ofs << "DIMENSIONS " << "21 21 21" << std::endl;
    ofs << "POINTS " << pointSet.size() << " INT" << std::endl;
    for(auto &i : pointSet)
    {
        ofs << i[0] << " " << i[1] << " " << i[2] << std::endl;
    }
    ofs << "CELL_DATA " << flatMap.size() << std::endl;
    ofs << "SCALARS cell_scalars FLOAT 1 " << std::endl;
    ofs << "LOOKUP_TABLE default" << std::endl;
    for(auto &i : flatMap)
    {
        ofs << i.second.second*scalar << std::endl;
    }
}


template<typename MapType, typename T_Functor>
auto
combineMapWithFunctor(
    const MapType & flatMap1,
    MapType & outputMap,
    T_Functor fun
)
{
    for(auto& elem : flatMap1)
    {
        auto pos = outputMap.find(elem.first);
        if(pos == outputMap.end()) {
            outputMap[elem.first] = elem.second;
        }
        else{
            pos->second.second = fun(pos->second.second, elem.second.second);
        }
    }
    return outputMap;
}


int main( )
{

//    std::string experimentName = "r12_nc100_PMMA_30fs";
//    int experimentTimeSteps = 88;
    std::string experimentName = "r06_gauss_nc08_p2sig3_PMMA_500fs";
    size_t experimentTimeSteps = 84;
    constexpr int dimension = 3;
    std::vector<std::string> particleTypes = {{ "C", "p", "O", "e"}};

    std::map< boost::array< size_t, dimension >, std::pair< boost::array< size_t, dimension >, size_t>> maximumMap;

    auto m = parsePythonOutput("/home/carli/dev/diplom/data/particlePatches/"+experimentName, "p", 0);
    auto flatFieldMap = fieldSizeMap3D( m );
    flatFieldMap = applyMapWeights( flatFieldMap, 2*3*4+3*4 + 4 );
    writeMapToVTK( flatFieldMap, "/tmp/field_grid.vtk" );

    for(size_t i = 0 ; i<experimentTimeSteps ; ++i){
        std::cout << "Timestep " << i << std::endl;
        decltype(maximumMap) memPerTimestep;
        for(auto particle : particleTypes)
        {
            auto m = parsePythonOutput("/home/carli/dev/diplom/data/particlePatches/"+experimentName, particle, i*500);
            auto flatMap = m;
            if(dimension==2) {
                auto flatMap = flatten3DParticleMap( m );
            }
            flatMap = applyMapWeights( flatMap, 4+3*4+3*4+4 );
            writeMapToVTK( flatMap, "/tmp/particle_" + particle + "_grid" + std::to_string( i * 500 ) + ".vtk" );
            combineMapWithFunctor(flatMap, memPerTimestep, std::plus<size_t>());
        }
        combineMapWithFunctor(flatFieldMap, memPerTimestep, std::plus<size_t>());
        writeMapToVTK( memPerTimestep, "/tmp/totalSize_grid" + std::to_string( i * 500 ) + ".vtk", 1. / 1024. / 1024. );
        combineMapWithFunctor(
            memPerTimestep,
            maximumMap,
            [ ]( auto a, auto b ) { return std::max( a, b ); }
        );
    }
    writeMapToVTK(
        maximumMap,
        "/tmp/maxTotalSizeMB_grid.vtk",
        1. / 1024. / 1024.
    );
    auto mymap = parsePythonOutput("/home/carli/dev/diplom/data/particlePatches/"+experimentName, "C", 10000);



    constexpr std::array<int, 3> superCellSize = {{ 8, 8, 4}};
    // granularity of the algorithm: how many supercells need to be grouped together
    constexpr std::array<int, 3> megaCellSize = {{ 4, 2, 2 }};
    constexpr std::array<int, 3> cellsInMegaCell = {{
        superCellSize[0] * megaCellSize[0],
        superCellSize[1] * megaCellSize[1],
        superCellSize[2] * megaCellSize[2]
    }};
    constexpr std::array<int, 3> domainElements = {{ 3680, 4624, 3680 }};
    constexpr std::array<int, 3> gridResolution = {{
        domainElements[0]/cellsInMegaCell[0],
        domainElements[1]/cellsInMegaCell[1],
        domainElements[2]/cellsInMegaCell[2]
    }};

    typedef boost::grid_graph<3> GridGraph;
    using Dimensional = boost::array<size_t, 3>;

    typedef boost::graph_traits<GridGraph> GridTraits;


    //Dimensional fullDimensions = { { 3680/16, 4624/16, 3680/16} };
    Dimensional fullDimensions{{gridResolution[0], gridResolution[1], gridResolution[2]}};
    GridGraph full_grid(fullDimensions);

    Dimensional patchDimensions{{20,20,20}};
    GridGraph patch_grid(patchDimensions);

    std::map<std::array<int,3>, size_t> patchMap;

    using indexMapType = boost::property_map<GridGraph, boost::vertex_index_t>::const_type;
    indexMapType indexMap(get(boost::vertex_index, patch_grid));
    boost::vector_property_map<VProp, indexMapType> dataMap(num_vertices(patch_grid), indexMap);

    indexMapType indexMap2(get(boost::vertex_index, full_grid));
    std::cout << "Elements in Map: "  << (num_vertices(full_grid)*4)/1024/1024./16/16/16 << " MB in size"<< std::endl;
    boost::vector_property_map<float, indexMapType> particleCountMap(num_vertices(full_grid), indexMap2);

    indexMapType indexMap3(get(boost::vertex_index, full_grid));
    std::cout << "Elements in Map: "  << (num_vertices(full_grid)*4)/1024/1024./16/16/16 << " MB in size"<< std::endl;
    boost::vector_property_map<double, indexMapType> memSizeMap(num_vertices(full_grid), indexMap3);

    indexMapType indexMap4(get(boost::vertex_index, patch_grid));
    boost::vector_property_map<double, indexMapType> patchMemSizeMap(num_vertices(patch_grid), indexMap4);

//    for(unsigned x(0); x<patchDimensions[0]; ++x)
//    {
////        GridTraits::vertex_descriptor first_vertex = vertex(0, patch_grid);
////        auto xPos = patch_grid.next(first_vertex,0,x);
//        for( unsigned y = 0; y < patchDimensions[1]; ++y )
//        {
////            auto yPos = patch_grid.next(xPos,1,y);
//            for( unsigned z = 0; z < patchDimensions[2]; ++z )
//            {
//                GridTraits::vertices_size_type currentV = get(boost::vertex_index_t(), patch_grid, {{x,y,z}});
//                GridTraits::vertex_descriptor zPos = vertex(currentV, patch_grid);
////                GridTraits::vertex_descriptor zPos = patch_grid.next(yPos,2,z);
//                VProp v;
//                // get extent from file
//                // get particle count from file
//                put(dataMap, zPos, v);

    std::array<std::array<std::size_t, gridResolution[1]>, gridResolution[0]> flatParticleCount;
    for(unsigned i=0; i<flatParticleCount.size(); ++i)
        for( auto && item : flatParticleCount[i] )
            item=0u;


    for(auto i : mymap)
    {
        VProp prop;
        prop.xoffset = i.first[0];
        prop.yoffset = i.first[1];
        prop.zoffset = i.first[2];
        prop.xextent = i.second.first[0];
        prop.yextent = i.second.first[1];
        prop.zextent = i.second.first[2];
        prop.particleCount = i.second.second;

        GridTraits::vertices_size_type patchElem = get(boost::vertex_index_t{}, patch_grid, {{prop.xoffset, prop.yoffset, prop.zoffset}});
        GridTraits::vertex_descriptor patchID = vertex(patchElem, patch_grid);
        put(patchMemSizeMap, patchID, (40.*prop.xextent*prop.yextent*prop.zextent+32.f*double(prop.particleCount)));

        // TODO: don't average particles, but take from 3D Histogram!
        float particleCount = prop.particleCount / (double(prop.xextent)*prop.yextent*double(prop.zextent));
        for(unsigned xFull{prop.xoffset} ; xFull < prop.xextent+prop.xoffset ; xFull+=cellsInMegaCell[0])
        {
            for(unsigned yFull{prop.yoffset} ; yFull < prop.yextent+prop.yoffset ; yFull+=cellsInMegaCell[1])
            {
                for(unsigned zFull{prop.zoffset} ; zFull < prop.zextent+prop.zoffset ; zFull+=cellsInMegaCell[2])
                {
                    GridTraits::vertices_size_type fullElem = get(
                        boost::vertex_index_t{},
                        full_grid,
                        {{
                            xFull/cellsInMegaCell[0],
                            yFull/cellsInMegaCell[1],
                            zFull/cellsInMegaCell[2]
                        }}
                    );

                    GridTraits::vertex_descriptor fullID = vertex(fullElem, full_grid);
                    put(particleCountMap, fullID, particleCount);
                    put(memSizeMap, fullID, (4096.f*40.f+32.f*double(particleCount)));
                }
            }
        }

    }







    //            }
//        }
//    }



//    using ComponentGridGraph = dodo::graph::SimpleBGL<VProp,EProp>;
//    ComponentGridGraph cgraph;


    return 0;
}
