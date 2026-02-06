#include <stdexcept>
#include "../NetSim (produkty i ich przechowywanie) - rozwiązanie zadań/incld/storage_types.hxx"

Package PackageQueue::pop() {
    if (this->empty()) {
        throw std::out_of_range("Package queue is empty");
    }
    if (package_queue_type_ == PackageQueueType::LIFO) {
        Package r = std::move(package_queue_.back());
        package_queue_.pop_back();
        return r;
    } else {
        Package r = std::move(package_queue_.front());
        package_queue_.pop_front();
        return r;
    }
}
