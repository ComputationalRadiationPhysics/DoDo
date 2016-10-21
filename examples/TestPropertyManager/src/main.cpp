#include <iostream>


#include <map>
#include <boost/property_map/property_map.hpp>
#include <boost/property_map/dynamic_property_map.hpp>

#include <dodo/utility/tree_id.hpp>
#include <dodo/utility/PropertyManager.hpp>
#include <dodo/model/hardware/property/VertexType.hpp>



int main( )
{

    using namespace dodo::utility;
    using namespace dodo::model::hardware::property;
    std::map<TreeID, VertexType > internal_nameMap;

    boost::associative_property_map< std::map<TreeID, VertexType>> nameMap(internal_nameMap);

    PropertyManager pm;
    pm.registerProperty("name", nameMap);

    TreeID root;

//    boost::dynamic_properties dp;
//    dp.property(std::string("name"), nameMap);
    pm.set("name", root, VertexType::STRUCTURAL);

    VertexType v = pm.get< VertexType >(
        "name",
        root
    );
    std::cout << v << std::endl;

//    put(std::string("name"), dp, root, VertexType::STRUCTURAL);
//    VertexType temp =  get<VertexType>(std::string("name"), dp, root);



    return 0;
}