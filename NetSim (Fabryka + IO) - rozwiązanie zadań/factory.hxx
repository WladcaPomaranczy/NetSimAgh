#ifndef FACTORY_HXX
#define FACTORY_HXX

#include <istream>
#include <list>
#include <algorithm>

#include "types.hxx"
#include "nodes.hxx"

// =====================
// NodeCollection
// =====================
template <class Node>
class NodeCollection {
public:
    using container_t = std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    iterator begin() { return nodes_.begin(); }
    iterator end() { return nodes_.end(); }
    const_iterator begin() const { return nodes_.begin(); }
    const_iterator end() const { return nodes_.end(); }
    const_iterator cbegin() const { return nodes_.cbegin(); }
    const_iterator cend() const { return nodes_.cend(); }

    void add(Node&& node) {
        nodes_.push_back(std::move(node));
    }

    iterator find_by_id(ElementID id) {
        return std::find_if(
            nodes_.begin(), nodes_.end(),
            [id](const Node& n) { return n.get_id() == id; }
        );
    }

    const_iterator find_by_id(ElementID id) const {
        return std::find_if(
            nodes_.cbegin(), nodes_.cend(),
            [id](const Node& n) { return n.get_id() == id; }
        );
    }

    void remove_by_id(ElementID id) {
        auto it = find_by_id(id);
        if (it != nodes_.end())
            nodes_.erase(it);
    }

private:
    container_t nodes_;
};

// =====================
// NodeColor
// =====================
enum class NodeColor {
    UNVISITED,
    VISITED,
    VERIFIED
};

// =====================
// Factory
// =====================
class Factory {
public:
    bool is_consistent() const;

    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);

    void add_ramp(Ramp&& ramp);
    void remove_ramp(ElementID id);
    Ramp* find_ramp_by_id(ElementID id);

    void add_worker(Worker&& worker);
    void remove_worker(ElementID id);
    Worker* find_worker_by_id(ElementID id);

    void add_storehouse(Storehouse&& storehouse);
    void remove_storehouse(ElementID id);
    Storehouse* find_storehouse_by_id(ElementID id);

    // =====ITERATORY=====

    auto ramp_cbegin() const { return ramps_.cbegin(); }
    auto ramp_cend()   const { return ramps_.cend(); }

    auto worker_cbegin() const { return workers_.cbegin(); }
    auto worker_cend()   const { return workers_.cend(); }

    auto storehouse_cbegin() const { return storehouses_.cbegin(); }
    auto storehouse_cend()   const { return storehouses_.cend(); }

private:
    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;

    template <class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);
};

// =====================
// IO
// =====================
Factory load_factory_structure(std::istream& is);

template <class Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id)
{
    for (auto& node : collection) {
        node.receiver_preferences_.remove_receiver(
            find_worker_by_id(id)
        );
        node.receiver_preferences_.remove_receiver(
            find_storehouse_by_id(id)
        );
    }
}


#endif // FACTORY_HXX
