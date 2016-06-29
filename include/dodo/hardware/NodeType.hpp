#pragma once

namespace dodo
{
    namespace hardware
    {

        enum class NodeType
        {
            COMPUTE,
            MEMORY,
            CACHE,
            STRUCTURAL,
            INTERCONNECT,
            _count
        };

        std::ostream& operator<<(std::ostream& ostream1, const NodeType& n)
        {
            switch(n)
            {
                case NodeType::COMPUTE:
                    ostream1 << "Compute";
                    break;
                case NodeType::MEMORY:
                    ostream1 << "Memory";
                    break;
                case NodeType::CACHE:
                    ostream1 << "Cache";
                    break;
                case NodeType::STRUCTURAL:
                    ostream1 << "Structural";
                    break;
                case NodeType::INTERCONNECT:
                    ostream1 << "Interconnect";
                    break;
                default:
                    throw std::runtime_error("Unknown NodeType!");

            }
            return ostream1;
        }

        std::string to_string(const NodeType & n)
        {
            std::stringstream ss;
            ss << n;
            return ss.str();
        }




    } /* graph */
} /* dodo */

namespace boost{
    template<>
    dodo::hardware::NodeType lexical_cast(const std::string & s) {
        if(s == "Compute")
            return dodo::hardware::NodeType::COMPUTE;
        if(s == "Memory")
            return dodo::hardware::NodeType::MEMORY;
        if(s == "Cache")
            return dodo::hardware::NodeType::CACHE;
        if(s == "Structural")
            return dodo::hardware::NodeType::STRUCTURAL;
        if(s == "Interconnect")
            return dodo::hardware::NodeType::INTERCONNECT;
        throw std::runtime_error("Unknown NodeType!");
    }
} /* boost */


