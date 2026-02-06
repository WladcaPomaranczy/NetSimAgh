#include "../NetSim (produkty i ich przechowywanie) - rozwiązanie zadań/incld/package.hxx"

#include <stdexcept>
std::set<ElementID> Package::free_ids = {};
std::set<ElementID> Package::assigned_ids = {};

Package::Package(){
    if (assigned_ids.empty() && free_ids.empty()) {
        id_ = 1;
    }
    else if (free_ids.empty()) {
        id_ = *assigned_ids.rbegin() + 1;
    }
    else {
        id_ = *free_ids.begin();
        free_ids.erase(id_);
    }
    assigned_ids.insert(this->get_id());
}

Package::Package(ElementID id){
    if (assigned_ids.find(id) != assigned_ids.end()) {
        throw std::runtime_error("Package already assigned");
    }
    id_ = id;
    free_ids.erase(id);
    assigned_ids.insert(id);
}

Package::Package(Package && other) noexcept{
    const ElementID id = other.get_id();
    other.id_ = -1;
    id_ = id;
}

Package &Package::operator=(Package &&other) noexcept {
    if (this != &other) {
        if (id_ != -1) {
            assigned_ids.erase(id_);
            free_ids.insert(id_);
        }
        id_ = other.get_id();
        other.id_ = -1;
    }
    return *this;
}

Package::~Package() {
    if (this->get_id() != -1){
        assigned_ids.erase(this->get_id());
        free_ids.insert(this->get_id());
    }
}