#pragma once

#include "package.hxx"
#include <deque>
#include <list>
#include <memory>
#include <stdexcept>

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageQueue {
public:
    virtual void push(Package&& p) = 0;
    virtual Package pop() = 0;
    virtual bool empty() const = 0;

    virtual PackageQueueType get_queue_type() const = 0;

    virtual ~IPackageQueue() = default;
};

class PackageQueue : public IPackageQueue {
public:
    explicit PackageQueue(PackageQueueType type) : type_(type) {}

    void push(Package&& p) override { data_.push_back(std::move(p)); }

    Package pop() override {
        if (data_.empty()) throw std::out_of_range("PackageQueue::pop on empty");

        if (type_ == PackageQueueType::FIFO) {
            Package p = std::move(data_.front());
            data_.pop_front();
            return p;
        } else {
            Package p = std::move(data_.back());
            data_.pop_back();
            return p;
        }
    }

    bool empty() const override { return data_.empty(); }

    PackageQueueType get_queue_type() const override {
        return type_;
    }

private:
    PackageQueueType type_;
    std::deque<Package> data_;
};
class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& p) = 0;

    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual bool empty() const = 0;

    virtual ~IPackageStockpile() = default;
};

class PackageStockpile : public IPackageStockpile {
public:
    void push(Package&& p) override { data_.push_back(std::move(p)); }

    const_iterator begin() const override { return data_.begin(); }
    const_iterator end() const override { return data_.end(); }
    const_iterator cbegin() const override { return data_.cbegin(); }
    const_iterator cend() const override { return data_.cend(); }

    bool empty() const override { return data_.empty(); }

private:
    std::list<Package> data_;
};