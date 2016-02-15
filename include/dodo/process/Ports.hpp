#pragma once

namespace dodo
{
namespace process
{

template<typename T_Edges>
class Ports
{
    T_Edges edges;

public:
    template<typename T_Data>
    auto send(int portnumber, const T_Data& data)
    {
        return std::get<portnumber>(this->edges) << data;
    }

    template<typename T_Data>
    T_Data recv(int portnumber)
    {
        T_Data result;
        std::get<portnumber>(this->edges) >> result;
        return result;
    }
};

template<typename T_OutEdges>
class OutPorts : private Ports<T_OutEdges>
{
public:
    using Ports<T_OutEdges>::send;
};

template<typename T_InEdges>
class InPorts : private Ports<T_InEdges>
{
public:
    using Ports<T_InEdges>::recv;
};

} /* process */
} /* dodo */

