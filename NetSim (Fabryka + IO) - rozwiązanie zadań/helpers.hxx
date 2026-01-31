#ifndef HELPERS_HPP_
#define HELPERS_HPP_

#include <functional>
#include <random>

#include <ostream>
#include <istream>

#include "types.hxx"

class Factory;

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;

// ===== IO =====
void save_factory_structure(const Factory& factory, std::ostream& os);
Factory load_factory_structure(std::istream& is);

#endif /* HELPERS_HPP_ */