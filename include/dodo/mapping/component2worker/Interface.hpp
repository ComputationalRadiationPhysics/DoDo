#pragma once

#include <memory>
#include <algorithm>
#include <string>

#include <dodo/model/worker/Model.hpp>
#include <dodo/utility/OneToNMap.hpp>
#include <dodo/model/routine/Abstraction.hpp>



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
        //using DataModel = model::data::Abstraction< T_SimDom >;
        using RoutineModel = model::routine::Abstraction< T_SimDom >;
        //using DataID = model::data::DataDomain::DataID;
        using ComponentID = typename RoutineModel::ComponentID;
        using WorkerModel = model::worker::Model;
        using WorkerID = WorkerModel::WorkerID;

        std::shared_ptr< RoutineModel > routineModel;
        std::shared_ptr< WorkerModel > workerModel;

    private:
        // the string denotes the name of the data domain that is mapped
        utility::OneToNMap<
            WorkerID,
            ComponentID
        > mapping;

    public:
        Interface(
            const std::shared_ptr< RoutineModel > & proutineModel,
            const std::shared_ptr< WorkerModel > & pworkerModel
        ) :
            routineModel( proutineModel ),
            workerModel( pworkerModel ),
            mapping{}
        { }

        Interface(
            const std::shared_ptr< RoutineModel > & proutineModel,
            const std::shared_ptr< WorkerModel > & pworkerModel,
            const std::map<
                WorkerID,
                std::vector< ComponentID >
            > & p_mapping
        ) :
            routineModel( proutineModel ),
            workerModel( pworkerModel ),
            mapping( p_mapping )
        { }


        auto
        getComponentsOfWorker(
            const WorkerID w
        ) const -> std::vector< ComponentID >
        {
            return mapping.one2n.at( w );
        }

        auto
        getWorkerHoldingComponent(
            const ComponentID c
        ) -> WorkerID
        {
            return mapping.n2one.at( c );
        }

        auto
        print( )
        {
            for(auto & worker : mapping.one2n)
            {
                std::cout << worker.first << ":";
                for(auto & comp : worker.second)
                {
                    std::cout << " " << comp;
                }
                std::cout << std::endl;
            }
        }


    };

} /* component2worker */
} /* mapping */
} /* dodo */
