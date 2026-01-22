#include "factory.hxx"
////////////////////////////////////////////////////////
#include <map>
enum class NodeColor { UNVISITED, VISITED, VERIFIED };

bool is_network_consistent() const
{
    /* Użyj algorytmu DFS do przejścia grafu, dla każdej rampy. */
    std::map<const PackageSender*, NodeColor>color;

    for(const auto& ramp : ramps_) {
        color[&ramp]= NodeColor::UNVISITED
    }
    for(const auto& work : workers_) {
        color[&ramp]= NodeColor::UNVISITED

    }


}

///////////////////////////


