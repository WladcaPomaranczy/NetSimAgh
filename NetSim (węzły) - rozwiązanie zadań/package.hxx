#pragma once
#include "types.hxx"

#include <set>

class Package {
public:
    Package();
    explicit Package(ElementID id) : id_(id) {}

    Package(const Package&) = delete;
    Package& operator=(const Package&) = delete;

    Package(Package&& other) noexcept;
    Package& operator=(Package&& other) noexcept;

    ~Package();

    ElementID get_id() const { return id_; }

private:
    ElementID id_{0};

    static std::set<ElementID> freed_ids_;
    static ElementID next_id_;
};
