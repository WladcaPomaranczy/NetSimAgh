#include "nodes.hxx"



void ReceiverPreferences::rescale_to_equal_() {
    if (preferences_.empty()) return;
    const double p = 1.0 / static_cast<double>(preferences_.size());
    for (auto& [_, w] : preferences_) w = p;
}

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    if (!r) return;
    preferences_[r] = 1.0;
    rescale_to_equal_();
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    if (!r) return;
    preferences_.erase(r);
    rescale_to_equal_();
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    if (preferences_.empty()) return nullptr;

    const double x = pg_(); // [0,1)
    double acc = 0.0;
    for (const auto& [recv, p] : preferences_) {
        acc += p;
        if (x <= acc) return recv;
    }
    return preferences_.rbegin()->first;
}



void PackageSender::push_package(Package&& p) {
    if (sending_buffer_.has_value()) {
        throw std::logic_error("Sending buffer already occupied");
    }
    sending_buffer_.emplace(std::move(p));
}

void PackageSender::send_package() {
    if (!sending_buffer_.has_value()) return;

    IPackageReceiver* r = receiver_preferences_.choose_receiver();
    if (!r) {
        throw std::logic_error("No receivers available");
    }

    r->receive_package(std::move(*sending_buffer_));
    sending_buffer_.reset();
}



void Ramp::deliver_goods(Time t) {
    if (delivery_interval_ < 1) throw std::logic_error("delivery_interval must be >= 1");
    if ((t - 1) % delivery_interval_ == 0) {
        push_package(Package{});
    }
}



void Worker::do_work(Time t) {

    if (!processing_buffer_.has_value() && !queue_->empty()) {
        processing_buffer_.emplace(queue_->pop());
        package_processing_start_time_ = t;
    }


    if (processing_buffer_.has_value()) {
        const Time elapsed = t - package_processing_start_time_ + 1;
        if (elapsed >= processing_duration_) {

            push_package(std::move(*processing_buffer_));
            processing_buffer_.reset();
            package_processing_start_time_ = 0;
        }
    }
}