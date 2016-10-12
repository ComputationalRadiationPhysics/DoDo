#pragma once

#include <dodo2.hpp>

template< typename T_DataModel >
class ComponentB : public dodo::model::routine::ComponentBase< T_DataModel >
{
public:

    virtual
    float
    effort(
        std::shared_ptr<T_DataModel> dataModel
    ) override
    {
        return 20 * dataModel->template getProperty<float>(
           this->inData[0],
           "sizeInKB"
       );
       // return 0;
    }

};

