#include "package.hxx"
#include <stdexcept>

Package::Package(){
    if (!free_ids.empty()) {
        id_ = *free_ids.begin();
        free_ids.erase(id_);
        assigned_ids.insert(id_);
    }
    else {
        id_ = *assigned_ids.end() + 1;
    }
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
    id_ = other.id_;
    other.id_ = NULL;
}

Package &Package::operator=(Package &&other) noexcept {
    if (this != &other) {
        id_ = other.id_;
        other.id_ = NULL;
    }
    return *this;
}

Package::~Package() {
    assigned_ids.erase(id_);
    free_ids.insert(id_);
}
