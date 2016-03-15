#pragma once


#include <memory>

#include <boost/hana/ext/std/tuple.hpp>

#include <dout/dout.hpp>
#include <utility>
#include <tuple>

#include "dodo/utility/tree_id.hpp"
#include "attributes.hpp"
#include "InterconnectGraph.hpp"


namespace dodo
{
    namespace hardware
    {

        template<
            typename T_LocalProperties,
            typename T_InterconnectProperties
        >
        class HardwareGraphVertex
        {
            dout::Dout &dout;
            using TreeID = utility::TreeID;

        public:
            using Properties = T_LocalProperties;
            using InterconnectProperties = T_InterconnectProperties;
            using ConsistsOfStructure = std::vector<HardwareGraphVertex>;
            using InterconnectGraph_t = InterconnectGraph<T_InterconnectProperties>;
            using InterconnectID = typename InterconnectGraph_t::VertexID;
            using Mapping = std::map<TreeID, InterconnectID>;

            TreeID id;
            std::shared_ptr<InterconnectGraph_t> interconnectGraph;
            Properties properties;
            ConsistsOfStructure children;
            // static constexpr auto t1 = std::tuple_cat(std::tuple<>(), Properties());
            // static constexpr auto t2 = std::tuple_cat(Properties());

            HardwareGraphVertex(
                TreeID i,
                std::shared_ptr<InterconnectGraph_t> a
            ) :
                dout{dout::Dout::getInstance()},
                id{i},
                interconnectGraph{a}
            {
                interconnectGraph->add(id);
            }


            template<
                typename T_Child,
                typename... T_Args
            >
            TreeID createChild(T_Args &&... args)
            {
                dout(dout::Flags::DEBUG) << "in createChild, id = " << id << std::endl;
                TreeID consistID{id.genChildID()};
                dout(dout::Flags::DEBUG) << "                consistId = " << consistID << std::endl;

                T_Child child{consistID, interconnectGraph, std::forward<T_Args>(args)...};
                children.push_back(child);

                return consistID;
            }


            void printLocalChildren()
            {
                dout(dout::Flags::INFO) << id << std::endl;
                for (const auto &c : children)
                {
                    dout(dout::Flags::INFO) << "\\___" << c.id << std::endl;
                }
            }


            void printAllChildren(const size_t offset = 0) const
            {
                std::stringstream indent;
                for (auto i(0u); i < offset + id.get().size(); ++i)
                {
                    indent << " ";
                }

                dout(dout::Flags::INFO, offset == 0) << id << " ---> " << interconnectGraph->mapping
                    .at(id) << std::endl;
                for (auto i(0u); i < children.size(); ++i)
                {
                    dout(dout::Flags::INFO) << indent.str() << "\\___";
                    children[i].printAllChildren(offset + 4);
                    if (i + 1 == children.size())
                    {
                        dout(dout::Flags::INFO) << std::endl;
                    }
                }

            }


            template<typename T>
            T &getProperty()
            {
                constexpr size_t tupleIndex{utility::tuple_index<Properties, T>::value};
                static_assert(static_cast<int>(tupleIndex) >= 0, "This property does not exist");
                return std::get<tupleIndex>(properties);
            }


            template<typename T>
            void setProperty(const T t)
            {
                getProperty<T>() = t;
            }


            virtual ~HardwareGraphVertex() = default;


        };

    } /* graph */
} /* dodo */
