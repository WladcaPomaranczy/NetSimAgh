#include "helpers.hxx"
#include "factory.hxx"

#include <cstdlib>
#include <random>
#include <ostream>
#include <string>
#include <sstream>

// Do generowania wysokiej jakości ciągów liczb pseudolosowych warto użyć
// zaawansowanych generatorów, np. algorytmu Mersenne Twister.
// zob. https://en.cppreference.com/w/cpp/numeric/random
std::random_device rd;
std::mt19937 rng(rd());

double default_probability_generator() {
    // Generuj liczby pseudolosowe z przedziału [0, 1); 10 bitów losowości.
    return std::generate_canonical<double, 10>(rng);
}

std::function<double()> probability_generator = default_probability_generator;

// ===== FACTORY IO =====

void save_factory_structure(const Factory& factory, std::ostream& os) {
    // ===== RAMPS =====
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        os << "LOADING_RAMP id=" << it->get_id()
           << " delivery-interval=" << it->get_delivery_interval()
           << "\n";
    }

    // ===== WORKERS =====
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        os << "WORKER id=" << it->get_id()
           << " processing-time=" << it->get_processing_duration()
           << " queue-type="
           << (it->get_queue()->get_queue_type() == PackageQueueType::FIFO ? "FIFO" : "LIFO")
           << "\n";
    }

    // ===== STOREHOUSES =====
    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        os << "STOREHOUSE id=" << it->get_id() << "\n";
    }

    // ===== LINKS: RAMPS =====
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        for (const auto& [receiver, prob] : it->receiver_preferences().get_preferences()) {
            os << "LINK src=ramp-" << it->get_id()
               << " dest="
               << (receiver->get_receiver_type() == ReceiverType::WORKER ? "worker-" : "store-")
               << receiver->get_id()
               << "\n";
        }
    }

    // ===== LINKS: WORKERS =====
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        for (const auto& [receiver, prob] : it->receiver_preferences().get_preferences()) {
            os << "LINK src=worker-" << it->get_id()
               << " dest="
               << (receiver->get_receiver_type() == ReceiverType::WORKER ? "worker-" : "store-")
               << receiver->get_id()
               << "\n";
        }
    }
}

Factory load_factory_structure(std::istream& is) {
    Factory factory;
    std::string line;

    while (std::getline(is, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string type;
        iss >> type;

        // ===== RAMP =====
        if (type == "LOADING_RAMP") {
            std::string token;
            ElementID id;
            Time interval;

            iss >> token; // id=1
            id = std::stoi(token.substr(3));

            iss >> token; // delivery-interval=3
            interval = std::stoi(token.substr(18));

            factory.add_ramp(Ramp(id, interval));
        }

        // ===== WORKER =====
        else if (type == "WORKER") {
            std::string token;
            ElementID id;
            TimeOffset pd;
            PackageQueueType qt;

            iss >> token; // id=2
            id = std::stoi(token.substr(3));

            iss >> token; // processing-time=3
            pd = std::stoi(token.substr(16));

            iss >> token; // queue-type=FIFO
            std::string qt_str = token.substr(11);
            qt = (qt_str == "FIFO") ? PackageQueueType::FIFO
                                    : PackageQueueType::LIFO;

            auto queue = std::make_unique<PackageQueue>(qt);
            factory.add_worker(Worker(id, pd, std::move(queue)));
        }

        // ===== STOREHOUSE =====
        else if (type == "STOREHOUSE") {
            std::string token;
            ElementID id;

            iss >> token; // id=5
            id = std::stoi(token.substr(3));

            factory.add_storehouse(Storehouse(id));
        }

        // ===== LINK =====
        else if (type == "LINK") {
            std::string src, dest;
            iss >> src >> dest;

            auto src_type = src.substr(4, src.find('-') - 4);
            auto src_id = std::stoi(src.substr(src.find('-') + 1));

            auto dest_type = dest.substr(5, dest.find('-') - 5);
            auto dest_id = std::stoi(dest.substr(dest.find('-') + 1));

            PackageSender* sender = nullptr;
            IPackageReceiver* receiver = nullptr;

            if (src_type == "worker")
                sender = factory.find_worker_by_id(src_id);
            else
                sender = factory.find_ramp_by_id(src_id);

            if (dest_type == "worker")
                receiver = factory.find_worker_by_id(dest_id);
            else
                receiver = factory.find_storehouse_by_id(dest_id);

            if (sender && receiver)
                sender->receiver_preferences().add_receiver(receiver);
        }
    }

    return factory;
}