#include "package.hxx"

std::set<ElementID> Package::freed_ids_{};
ElementID Package::next_id_ = 1;

Package::Package() {
    if (!freed_ids_.empty()) {
        id_ = *freed_ids_.begin();
        freed_ids_.erase(freed_ids_.begin());
    } else {
        id_ = next_id_++;
    }
}

Package::Package(Package&& other) noexcept : id_(other.id_) {
    other.id_ = 0;
}

Package& Package::operator=(Package&& other) noexcept {
    if (this == &other) return *this;

    if (id_ != 0) freed_ids_.insert(id_);

    id_ = other.id_;
    other.id_ = 0;
    return *this;
}

Package::~Package() {
    if (id_ != 0) freed_ids_.insert(id_);
}