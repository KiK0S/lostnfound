
#pragma once
#include <random>
#include "serializable_system.hpp"

namespace rnd {

class RandomGenerator {
public:
	RandomGenerator() : generator(seed()) {}

	int getInt(int min, int max) {
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(generator);
	}

	double getDouble(double min, double max) {
		std::uniform_real_distribution<double> distribution(min, max);
		return distribution(generator);
	}

private:
	std::random_device seed;
	std::mt19937 generator;
};

RandomGenerator randomGenerator;

int getInt(int min, int max) {
	return randomGenerator.getInt(min, max);
}

double getDouble(double min, double max) {
	return randomGenerator.getInt(min, max);
}

struct PerlinNoise : public serialization::Serializable, serialization::Deserializable  {
	PerlinNoise(): n(5), alpha(0.5), omega(0.5) {}

	double f(double x) {
		return std::sin(x);
	}

	double operator()(double x) {
		double res = 0;
		double alpha_power = 1.0;
		double omega_power = 1.0;
		for (int i = 0; i < n; i++) {
			res += alpha_power * f(omega_power * x);
			alpha_power *= alpha;
			omega_power *= omega;
		}
		return res;
	}

	double operator()(double x, double y) {
		return (*this)(x) + (*this)(y);
	}

	void serialize(std::stringstream& ss) {
		serialization::serialize(ss, (size_t) n);
		serialization::serialize(ss, alpha);
		serialization::serialize(ss, omega);
	}

	void deserialize(std::string& s) {
		n = serialization::deserializeInt(s);
		alpha = serialization::deserializeFloat(s);
		omega = serialization::deserializeFloat(s);
	}

	int n;
	double alpha;
	double omega;
};

}