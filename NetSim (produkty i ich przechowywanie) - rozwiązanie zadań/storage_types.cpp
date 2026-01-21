#include "storage_types.hxx"

#include <stdexcept>

#include "package.hxx"

Package PackageQueue::pop(){
    if (empty()){throw std::out_of_range("Package queue is empty");}
    Package r;
    switch (package_queue_type_) {
        case PackageQueueType::LIFO:
            r = std::move(package_queue_.back());
            package_queue_.pop_back();
            break;
        case PackageQueueType::FIFO:
            r = std::move(package_queue_.front());
            package_queue_.pop_front();
            break;
    }
    return r;
}
