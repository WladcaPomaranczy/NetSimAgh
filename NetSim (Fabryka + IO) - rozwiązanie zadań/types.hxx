#include <functional>

#pragma once
enum class ReceiverType {
    WORKER,
    STOREHOUSE
};

using ElementID = int;
using Time = int;
using TimeOffset = int;
using ProbabilityGenerator = std::function<double()>;