#pragma once


#include <memory>
#include <vector>

#include <boost/hana.hpp>

#include <dodo/graph/AttributeMapStore.hpp>
#include <dodo/hardware/TagInfo.hpp>


namespace dodo
{
    namespace hardware
    {

        template<typename... AttributeTypes>
        class Property
        {
            using StoreType = graph::AttributeMapStore<AttributeTypes...>;
            using Handle = typename StoreType::Handle;
            std::weak_ptr<StoreType> attributeMapStore;
            std::vector<Handle> handles;

        public:
            TagInfo tagInfo;


            Property()
                :
                attributeMapStore(), handles(StoreType::AttributeCount, nullptr)
            {}


            Property(std::weak_ptr<StoreType> p)
                :
                attributeMapStore(p), handles(StoreType::AttributeCount, nullptr)
            {}


            template<typename... Attributes>
            Property(std::weak_ptr<StoreType> p, Attributes... attributes)
                :
                attributeMapStore(p), handles(StoreType::AttributeCount, nullptr)
            {
                setEntry(attributes...);
            }


            template<typename H, typename... T>
            auto setEntry(H head, T... tail)
            {
                Handle newHandle = attributeMapStore.lock()->addEntry(head);
                handles[newHandle->mapID] = newHandle;
                setEntry(tail...);
            }


            auto setEntry()
            {}


            template<typename T>
            T getEntry()
            {
                auto mapID = graph::getMapID<T>(attributeMapStore);
                return boost::get<T>(attributeMapStore.lock()->getEntry(handles[mapID]));
            }


            template<typename T>
            bool hasEntry()
            {
                auto mapID = graph::getMapID<T>(attributeMapStore);
                return nullptr != (attributeMapStore.lock()->getEntry(handles[mapID]));
            }


            Property clone()
            {
                Property cloned(attributeMapStore);

                for (auto handle : handles)
                {
                    if (handle != nullptr)
                    {
                        auto entry = attributeMapStore.lock()->getEntry(handle);
                        auto newHandle = attributeMapStore.lock()->addEntry(handle->mapID, entry);
                        cloned.handles[newHandle->mapID] = newHandle;
                    }
                }
                return cloned;
            }


            // template parameters that we want to convert to
            template<typename... Attrs>
            void remapHandlesAndTags(Property<Attrs...> &destination)
            {

                destination.tagInfo = tagInfo;

                namespace hana = boost::hana;

                constexpr auto destSet = hana::to_set(hana::tuple_t<Attrs...>);
                constexpr auto srcSet = hana::to_set(hana::tuple_t<AttributeTypes...>);
                constexpr auto intersection = hana::intersection(srcSet, destSet);

                hana::for_each(
                    intersection,
                    [this, &destination](auto t)
                    {
                        using T = typename decltype(t)::type;

                        if (!attributeMapStore.expired())
                        {
                            if (handles[graph::getMapID<T>(attributeMapStore)] != nullptr)
                            {
                                T entry = this->getEntry<T>();
                                destination.setEntry(entry);
                            }
                        }
                    }
                );


                // namespace mpl = boost::mpl;
                //
                // using DestSet = mpl::set<Attrs...>;
                // using SrcSet  = mpl::set<AttributeTypes...>;
                // using Intersection = typename mpl::copy_if<
                //     DestSet,
                //     mpl::has_key< SrcSet, mpl::_1 >,
                //     mpl::back_inserter< mpl::vector<> >
                // >::type;
                //
                // mpl::for_each<Intersection>(
                //     [this, &destination](auto t)
                //     {
                //         using T = decltype(t);
                //         if( !attributeMapStore.expired() && hasType<T>(attributeMapStore) )
                //         {
                //             if(handles[getMapID<T>(attributeMapStore)] != nullptr)
                //             {
                //                 T entry = this->getEntry<T>();
                //                 destination.setEntry(entry);
                //             }
                //         }
                //     }
                // );
            }
        };


    } /* graph */
} /* dodo */
