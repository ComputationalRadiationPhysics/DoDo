#pragma once

#include <memory>
#include <algorithm>

#include <boost/bimap.hpp>
#include <dodo2/model/worker/Model.hpp>
#include <dodo2/model/hardware/HardwareAbstraction.hpp>
#include <dodo2/model/hardware/HardwareAbstractionBase.hpp>
#include <dodo2/utility/OneToNMap.hpp>
#include <dodo2/graph/DataGraph.hpp>
#include <dodo2/model/data/Abstraction.hpp>


namespace dodo
{
namespace mapping
{
namespace data2worker
{


    class Interface
    {
        using DataModel = model::data::Abstraction;
        using DataID = model::data::DataDomain::DataID;
        using WorkerModel = model::worker::Model;
        using WorkerID = WorkerModel::WorkerID;

        std::map<
            std::string,
            utility::OneToNMap<
                WorkerID,
                DataID
            >
        > mapping;

    public:
        std::shared_ptr< DataModel > dataModel;
        std::shared_ptr< WorkerModel > workerModel;


        Interface(
            const std::shared_ptr< DataModel > & dataModel,
            const std::shared_ptr< WorkerModel > & workerModel
        ) :
            dataModel( dataModel ),
            workerModel( workerModel )
        { }

        Interface(
            const std::shared_ptr< DataModel > & dataModel,
            const std::shared_ptr< WorkerModel > & workerModel,
            const std::map<
                std::string,
                std::map<
                    WorkerID,
                    DataID
                >
            > & p_mapping
        ) :
            dataModel( dataModel ),
            workerModel( workerModel )
//            mapping( mapping )
        {
            for(auto a : p_mapping)
            {
                mapping[a.first].n2one = a.second;
            }

        }

        Interface(
            const std::shared_ptr< DataModel > & dataModel,
            const std::shared_ptr< WorkerModel > & workerModel,
            const std::map<
                std::string,
                std::map<
                    DataID,
                    std::vector< WorkerID >
                >
            > & p_mapping
        ) :
            dataModel( dataModel ),
            workerModel( workerModel )
//            mapping( mapping )
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
            return mapping[dataName].one2n[w];
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
