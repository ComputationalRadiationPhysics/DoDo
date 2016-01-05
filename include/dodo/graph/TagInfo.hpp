#pragma once

#include <vector>

#include <dodo/graph/NodeType.hpp>


namespace dodo
{
namespace graph
{

class TagInfo
{
    std::vector<unsigned> values;

public:

    TagInfo()
        : values(static_cast<int>(NodeType::_count), 0u)
    {}

    TagInfo(std::vector<unsigned> init)
        : values(init)
    {
        assert(values.size() == static_cast<size_t>(NodeType::_count));
    }

    TagInfo(std::vector<unsigned> &&init)
        : values(init)
    {
        assert(values.size() == static_cast<size_t>(NodeType::_count));
    }

    unsigned addNode(NodeType n)
    {
        assert(n != NodeType::_count);
        return ++values[static_cast<int>(n)];
    }

    unsigned removeNode(NodeType n)
    {
        assert(n != NodeType::_count);
        int pos = static_cast<int>(n);
        assert(values[pos] > 0);
        return --values[pos];
    }

    unsigned count(NodeType n)
    {
        assert(n != NodeType::_count);
        return values[static_cast<int>(n)];
    }

};

} /* graph */
} /* dodo */
