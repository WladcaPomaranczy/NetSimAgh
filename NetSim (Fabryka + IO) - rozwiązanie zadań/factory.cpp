#include "factory.hxx"
#include "nodes.hxx"


#include "types.hxx"

#include <map>
#include <stdexcept>

//FUNKCJA POMOCNICZA DFS
bool has_reachable_storehouse(
    const PackageSender* sender,
    std::map<const PackageSender*, NodeColor>& colors)
{
    if (colors[sender] == NodeColor::VERIFIED)
        return true;

    if (colors[sender] == NodeColor::VISITED)
        return false;

    colors[sender] = NodeColor::VISITED;

    const auto& prefs = sender->receiver_preferences().get_preferences();
    if (prefs.empty())
        return false;

    for (const auto& [receiver, prob] : prefs) {

        if (receiver->get_receiver_type() == ReceiverType::STOREHOUSE) {
            continue;
        }

        auto next_sender = dynamic_cast<const PackageSender*>(receiver);
        if (!next_sender)
            return false;

        if (!has_reachable_storehouse(next_sender, colors))
            return false;
    }

    colors[sender] = NodeColor::VERIFIED;
    return true;
}

//FACTORY
bool Factory::is_consistent() const
{
    for (const auto& r : ramps_) {
        std::map<const PackageSender*, NodeColor> colors;

        for (const auto& rr : ramps_)
            colors[&rr] = NodeColor::UNVISITED;
        for (const auto& w : workers_)
            colors[&w] = NodeColor::UNVISITED;

        if (!has_reachable_storehouse(&r, colors))
            return false;
    }
    return true;
}


void Factory::do_deliveries(Time t)
{
    for (auto& r : ramps_)
        r.deliver_goods(t);
}

void Factory::do_package_passing()
{
    for (auto& w : workers_)
        w.send_package();
}

void Factory::do_work(Time t)
{
    for (auto& w : workers_)
        w.do_work(t);
}

//ADD
void Factory::add_ramp(Ramp&& ramp)
{
    ramps_.add(std::move(ramp));
}

void Factory::add_worker(Worker&& worker)
{
    workers_.add(std::move(worker));
}

void Factory::add_storehouse(Storehouse&& storehouse)
{
    storehouses_.add(std::move(storehouse));
}

//FIND
Ramp* Factory::find_ramp_by_id(ElementID id)
{
    auto it = ramps_.find_by_id(id);
    return it == ramps_.end() ? nullptr : &(*it);
}

Worker* Factory::find_worker_by_id(ElementID id)
{
    auto it = workers_.find_by_id(id);
    return it == workers_.end() ? nullptr : &(*it);
}

Storehouse* Factory::find_storehouse_by_id(ElementID id)
{
    auto it = storehouses_.find_by_id(id);
    return it == storehouses_.end() ? nullptr : &(*it);
}

//REMOVE
void Factory::remove_ramp(ElementID id)
{
    remove_receiver(workers_, id);
    ramps_.remove_by_id(id);
}

void Factory::remove_worker(ElementID id)
{
    remove_receiver(workers_, id);
    remove_receiver(ramps_, id);
    workers_.remove_by_id(id);
}

void Factory::remove_storehouse(ElementID id)
{
    remove_receiver(workers_, id);
    remove_receiver(ramps_, id);
    storehouses_.remove_by_id(id);
}



