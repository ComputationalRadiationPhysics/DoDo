#pragma once

#include <memory>
#include <algorithm>
#include <string>

#include <boost/bimap.hpp>
#include <dodo2/model/worker/Model.hpp>
#include <dodo2/model/hardware/HardwareAbstraction.hpp>
#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/utility/OneToNMap.hpp>
#include <dodo2/graph/DataGraph.hpp>
#include <dodo2/model/data/Abstraction.hpp>
#include <dodo2/utility/NToMMap.hpp>


namespace dodo
{
namespace mapping
{
namespace component2worker
{


    template< typename T_SimDom >
    class Interface
    {
    public:
        using DataModel = model::data::Abstraction< T_SimDom >;
        using DataID = model::data::DataDomain::DataID;
        using WorkerModel = model::worker::Model;
        using WorkerID = WorkerModel::WorkerID;

    private:
        // the string denotes the name of the data domain that is mapped
        std::map<
            std::string,
            utility::OneToNMap<
                WorkerID,
                DataID
            >
        > mapping;

    public:
        std::shared_ptr< DataModel > ;
        std::shared_ptr< WorkerModel > workerModel;


        Interface(
            const std::shared_ptr< DataModel > & pdataModel,
            const std::shared_ptr< WorkerModel > & pworkerModel
        ) :
            dataModel( pdataModel ),
            workerModel( pworkerModel )
        { }

        Interface(
            const std::shared_ptr< DataModel > & pdataModel,
            const std::shared_ptr< WorkerModel > & pworkerModel,
            const std::map<
                std::string,
                std::map<
                    DataID,
                    WorkerID
                >
            > & p_mapping
        ) :
            dataModel( pdataModel ),
            workerModel( pworkerModel )
        {
            for( auto a : p_mapping )
            {
                for( auto elem : a.second )
                {
                    mapping[a.first].addMapping(
                        elem.second,
                        elem.first
                    );
                }
            }
        }

        Interface(
            const std::shared_ptr< DataModel > & pdataModel,
            const std::shared_ptr< WorkerModel > & pworkerModel,
            const std::map<
                std::string,
                std::map<
                    WorkerID,
                    std::vector< DataID >
                >
            > & p_mapping
        ) :
            dataModel( pdataModel ),
            workerModel( pworkerModel )
        {
            for( auto a : p_mapping )
            {
                for( auto elem : a.second )
                {
                    for( auto d : elem.second )
                    {
                        mapping[a.first].addMapping(
                            elem.first,
                            d
                        );
                    }
                }
            }
        }


        auto
        moveDataToWorker(
            const std::string & dataName,
            const DataID d,
            const WorkerID w
        )
        -> void
        {
            mapping[dataName].eraseMapping( d );
            mapping[dataName].addMapping(
                w,
                d
            );
        }

        auto
        getDataOfWorker(
            const std::string & dataName,
            const WorkerID w
        ) const -> std::vector< DataID >
        {
            return mapping.at( dataName ).one2n.at( w );
        }

        auto
        getWorkerHoldingData(
            const std::string & dataName,
            const DataID d
        ) -> WorkerID
        {
            return mapping[dataName].n2one.at( d );
        }

        auto
        print( )
        {
            for(auto & a : mapping)
            {
                std::cout << "Map " << a.first << std::endl;
                for(auto & elem : a.second.one2n)
                {
                    std::cout << elem.first << ":";
                    for(auto & data : elem.second)
                    {
                        std::cout << " " << data << std::endl;
                    }
                }
            }
        }


    };

} /* component2worker */
} /* mapping */
} /* dodo */
