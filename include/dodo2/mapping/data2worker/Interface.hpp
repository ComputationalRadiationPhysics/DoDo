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
namespace data2worker
{


    template<typename T_SimDom>
    class Interface
    {
        using DataModel = model::data::Abstraction< T_SimDom >;
        using DataID = model::data::DataDomain::DataID;
        using WorkerModel = model::worker::Model;
        using WorkerID = WorkerModel::WorkerID;

//        std::map<
//            std::string,
//            utility::OneToNMap<
//                WorkerID,
//                DataID
//            >
//        > mapping;

        // the string denotes the name of the data domain that is mapped
        std::map<
            std::string,
            utility::NToMMap<
                WorkerID,
                DataID
            >
        > mapping;

    public:
        std::shared_ptr< DataModel > dataModel;
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
                    WorkerID,
                    DataID
                >
            > & p_mapping
        ) :
            dataModel( pdataModel ),
            workerModel( pworkerModel )
        {
            for(auto a : p_mapping)
            {
                mapping[a.first].n2one = a.second;
            }

        }

        Interface(
            const std::shared_ptr< DataModel > & pdataModel,
            const std::shared_ptr< WorkerModel > & pworkerModel,
            const std::map<
                std::string,
                std::map<
                    DataID,
                    std::vector< WorkerID >
                >
            > & p_mapping
        ) :
            dataModel( pdataModel ),
            workerModel( pworkerModel )
        {
            for(auto a : p_mapping)
            {
                mapping[a.first].one2n = a.second;
            }

        }


        auto
        moveDataToWorker(
            const std::string& dataName,
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
            return mapping.at(dataName).one2n.at(w);
        }

        auto
        getWorkerHoldingData(
            const std::string & dataName,
            const DataID d
        ) -> WorkerID
        {
            return mapping[dataName].n2one.at(d);
        }


    };

} /* data2worker */
} /* mapping */
} /* dodo */
