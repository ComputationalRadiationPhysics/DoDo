#pragma once

#include <vector>

#include <dodo2/graph/CoordinateGraph.hpp>
#include "InData.hpp"


namespace dodo{
namespace model{
namespace routine{

    template< typename T_DataModel >
    struct ComponentBase
    {
    public:
        using PosID = data::SimulationDomain::Graph::VertexID;
        using DataModel = T_DataModel;
        PosID id;
        std::vector< InData > inData;
        std::vector< InData > outData;
//        std::string name;

        virtual
        float
        effort( std::shared_ptr<DataModel> ) = 0;

        virtual ~ComponentBase( ) {}

    };

    std::ostream &
    operator<<(
        std::ostream &,
        const std::vector< InData > &
    );
    std::ostream &
    operator<<(
        std::ostream & ostream1 ,
        const std::vector< InData > & inData
    )
    {
        ostream1 << inData.size() << std::endl;
        for(auto const & d : inData)
        {
            ostream1 << d << std::endl;
        }
        return ostream1;
    }


    std::istream &
    operator>>(
        std::istream &,
        std::vector< dodo::model::routine::InData > &
    );
    std::istream &
    operator>>(
        std::istream & stream,
        std::vector< dodo::model::routine::InData > & inData
    )
    {
        size_t s;
        stream >> s;
        stream.get();
        inData.reserve(s);
        for(size_t i = 0; i<s ; ++i)
        {
            stream >> inData[i];
            stream.get();
        }
        return stream;
    }


    template< typename T >
    std::ostream &
    operator<<(
        std::ostream & ostream1,
        const ComponentBase<T> & c
    )
    {
        ostream1 << c.id << std::endl;
        ostream1 << c.inData << std::endl;
        ostream1 << c.outData << std::endl;
        return ostream1;
    }

    template <typename T>
    std::istream& operator>>(
        std::istream & stream,
        ComponentBase< T > & c
    )
    {
        stream >> c.id;
        stream.get();
        stream >> c.inData;
        stream.get();
        stream >> c.outData;
        stream.get();
        return stream;
    }

    template< typename T >
    std::ostream & operator<<(
        std::ostream & ostream1,
        const std::shared_ptr<ComponentBase<T>> c
    )
    {
        ostream1 << *c;
        return ostream1;
    }


    template <typename T>
    std::istream&
    operator>>(std::istream& stream, std::shared_ptr<ComponentBase<T>> &p)
    {
        stream >> *p;
        return stream;
    }


} /* routine */
} /* model */
} /* dodo */


//namespace boost
//{
////    std::string
////    lexical_cast( const dodo::utility::TreeID & );
//
//    template<typename T>
//    std::string
//    lexical_cast( const dodo::model::routine::ComponentBase<T> & c )
//    {
//        return c.name;
//    }
//
//    template<typename T>
//    std::string
//    lexical_cast( const std::shared_ptr<dodo::model::routine::ComponentBase<T>> & c )
//    {
//        return c->name;
//    }
//
////    template<>
//    template<typename T>
//    std::shared_ptr<dodo::model::routine::ComponentBase<T>>
//    lexical_cast( const std::string & s )
//    {
//        return std::make_shared<dodo::model::routine::ComponentBase<T>>();
//    }
//
//    template<typename T>
//    dodo::model::routine::ComponentBase<T>
//    lexical_cast( const std::string & s )
//    {
//        return dodo::model::routine::ComponentBase<T>();
//    }
//} /* boost */

