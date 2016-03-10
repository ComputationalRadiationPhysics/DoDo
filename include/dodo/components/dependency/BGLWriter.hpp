#pragma once


#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/mpl/range_c.hpp>

#include <dodo.hpp>

#include "BGL.hpp"
#include "../../graph/SimpleBGLWriter.hpp"
#include "../../graph/detail/PrettyPrinter.hpp"



namespace dodo
{
namespace components
{
namespace dependency
{

template <typename T_Seq, typename T_Graph>
void member_iterator(boost::dynamic_properties& dp, T_Graph& g)
{
    using namespace boost;
    using Indices = mpl::range_c<unsigned, 0, fusion::result_of::size<T_Seq>::value>;
    fusion::for_each(
        Indices(),
        [ & ](auto i)
        {
            using I = decltype(i);
            dp.property(
                fusion::extension::struct_member_name<T_Seq, i>::call(),
                make_transform_value_property_map(
                    &graph::detail::prettyPrinter<
                        typename fusion::result_of::value_at<T_Seq, I>::type
                    >,
                    get(
                        dodo::utility::pointer_to_member_N<T_Seq, i>::value,
                        g
                    )
                )
            );
        }
    );
}



/**
 * Printer for BGL graph
 *
 * Prints the whole BGL graph including the necessary properties
 * for vertices and edges. The function writes to an output stream the
 * graph in the .graphml format.
 *
 * @param os    an output stream to write to
 * @param graph the BGL graph
 */
std::ostream &operator<<(std::ostream &os, const BGL &graph)
{
//    using IndexMap = std::map<BGL::VertexID, std::size_t>;
//    IndexMap im;
//    boost::associative_property_map<IndexMap> propMapIndex(im);
//    auto allV = graph.getVertices();
//    int index = 0;
//
//    // ensure monotone values for the node-IDs (necessary when using listS)
//    for (auto i = allV.first; i != allV.second; ++i)
//    {
//        propMapIndex[*i] = index++;
//    }
//

    // TODO: Find a way to iterate over all the members of a struct (property)
    // see:
    // http://stackoverflow.com/questions/12084781/c-iterate-into-nested-struct-field-with-boost-fusion-adapt-struct
    // http://stackoverflow.com/questions/7142532/c-iterating-through-all-of-an-objects-members
    // http://stackoverflow.com/questions/27687455/statically-iterate-over-all-members-of-a-c-struct

    // also read: http://stackoverflow.com/questions/34132273/adding-custom-properties-to-vertex-of-a-grid-in-boost-graph-library/34139227#34139227
    auto &g = *(graph.graph);
    boost::dynamic_properties dp;
    // extract all interesting properties

    member_iterator<Vertex>(dp, g);
    member_iterator<Edge>(dp, g);

//    dp.property("id",
//        boost::make_transform_value_property_map(
//            &graph::detail::prettyPrinter<decltype(Vertex::id)>,
//            boost::get(&Vertex::id, g)
//        )
//    );
//    dp.property("from_Port", boost::get(&Edge::from, g));
//    dp.property("to_Port", boost::get(&Edge::to, g));

//    write_graphml(os, g, propMapIndex, dp);
    graph::simpleBGLWriter(os, graph, dp);
    return os;
}


}
}
}