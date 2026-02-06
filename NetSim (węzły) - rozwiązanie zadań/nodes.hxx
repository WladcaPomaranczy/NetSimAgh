#pragma once

#include "helpers.hxx"
#include "storage_types.hxx"
#include "types.hxx"

#include <map>
#include <memory>
#include <optional>
#include <stdexcept>

class IPackageReceiver {
public:
    using const_iterator = IPackageStockpile::const_iterator;

    virtual void receive_package(Package&& p) = 0;


    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual ElementID get_id() const = 0;

    virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator)
        : pg_(std::move(pg)) {}

    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);

    IPackageReceiver* choose_receiver();

    const preferences_t& get_preferences() const { return preferences_; }


    const_iterator begin() const { return preferences_.begin(); }
    const_iterator end() const { return preferences_.end(); }
    const_iterator cbegin() const { return preferences_.cbegin(); }
    const_iterator cend() const { return preferences_.cend(); }

private:
    void rescale_to_equal_(); 

    preferences_t preferences_;
    ProbabilityGenerator pg_;
};

class PackageSender {
public:

    ReceiverPreferences receiver_preferences_;

    explicit PackageSender(ProbabilityGenerator pg = probability_generator)
        : receiver_preferences_(std::move(pg)) {}

    PackageSender(PackageSender&&) = default;
    PackageSender& operator=(PackageSender&&) = default;

    void send_package();
    void push_package(Package&& p);

    std::optional<Package>& get_sending_buffer() { return sending_buffer_; }
    const std::optional<Package>& get_sending_buffer() const { return sending_buffer_; }

protected:
    std::optional<Package> sending_buffer_;
};

class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di, ProbabilityGenerator pg = probability_generator)
        : PackageSender(std::move(pg)), id_(id), delivery_interval_(di) {}

    void deliver_goods(Time t);

    TimeOffset get_delivery_interval() const { return delivery_interval_; }
    ElementID get_id() const { return id_; }

private:
    ElementID id_;
    TimeOffset delivery_interval_;
};

class Worker : public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q,
           ProbabilityGenerator pg = probability_generator)
        : PackageSender(std::move(pg)),
          id_(id),
          processing_duration_(pd),
          queue_(std::move(q)) {
        if (!queue_) throw std::invalid_argument("Worker queue must not be null");
        if (processing_duration_ < 1) throw std::invalid_argument("processing_duration must be >= 1");
    }

    void do_work(Time t);


    void receive_package(Package&& p) override { queue_->push(std::move(p)); }
    ElementID get_id() const override { return id_; }


    const_iterator begin() const override { return empty_.begin(); }
    const_iterator end() const override { return empty_.end(); }
    const_iterator cbegin() const override { return empty_.cbegin(); }
    const_iterator cend() const override { return empty_.cend(); }

    TimeOffset get_processing_duration() const { return processing_duration_; }
    Time get_package_processing_start_time() const { return package_processing_start_time_; }

private:
    ElementID id_;
    TimeOffset processing_duration_;
    std::unique_ptr<IPackageQueue> queue_;

    std::optional<Package> processing_buffer_;
    Time package_processing_start_time_{0};

    static inline std::list<Package> empty_{};
};

class Storehouse : public IPackageReceiver {
public:
    explicit Storehouse(
        ElementID id,
        std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageStockpile>())
        : id_(id), stockpile_(std::move(d)) {
        if (!stockpile_) throw std::invalid_argument("Storehouse stockpile must not be null");
    }

    void receive_package(Package&& p) override { stockpile_->push(std::move(p)); }
    ElementID get_id() const override { return id_; }

    const_iterator begin() const override { return stockpile_->begin(); }
    const_iterator end() const override { return stockpile_->end(); }
    const_iterator cbegin() const override { return stockpile_->cbegin(); }
    const_iterator cend() const override { return stockpile_->cend(); }

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> stockpile_;
};
