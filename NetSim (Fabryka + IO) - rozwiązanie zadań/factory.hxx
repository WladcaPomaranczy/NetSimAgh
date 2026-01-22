#include <map>

class PackageSender;

#include "nodes.hxx"


enum class NodeColor { 
    UNVISITED, 
    VISITED, 
    VERIFIED 
};


bool has_reachable_storehouse(
    const PackageSender* sender, 
    std::map<const PackageSender*, 
    NodeColor>& node_colors);


class Factory {
public:
    bool is_consistent() const;
};
