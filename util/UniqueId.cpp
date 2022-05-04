// UniqueId.cpp : Defines the entry point for the console application.
//
#include <string>
#include <random>
#include <sstream>
#include "UniqueId.h"

using namespace std;

namespace uuid {

#ifdef CPPSTD20
	template<class T = std::mt19937, std::size_t N = T::state_size * sizeof(typename T::result_type)>
	T ProperlySeededRandomEngine() {
		std::random_device source;
		auto random_data = std::views::iota(std::size_t(), (N - 1) / sizeof(source()) + 1)
			| std::views::transform([&](auto) { return source(); });
		std::seed_seq seeds(std::begin(random_data), std::end(random_data));
		return T(seeds);
	}
#else
	template<class T = std::mt19937, std::size_t N = T::state_size * sizeof(typename T::result_type)>
	auto ProperlySeededRandomEngine() -> typename std::enable_if<N, T>::type {
		std::random_device source;
		std::random_device::result_type random_data[(N - 1) / sizeof(source()) + 1];
		std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
		std::seed_seq seeds(std::begin(random_data), std::end(random_data));
		return T(seeds);
	}
#endif
	//static std::random_device              rd;
	static std::mt19937                    gen(ProperlySeededRandomEngine());
	static std::uniform_int_distribution<> dis(0, 15);
	static std::uniform_int_distribution<> dis2(8, 11);

	std::string generate_uuid_v4() {
		std::stringstream ss;
		int i;
		ss << std::hex;
		for (i = 0; i < 8; i++) {
			ss << dis(gen);
		}
		ss << "-";
		for (i = 0; i < 4; i++) {
			ss << dis(gen);
		}
		ss << "-";
		for (i = 0; i < 3; i++) {
			ss << dis(gen);
		}
		ss << "-";
		ss << dis2(gen);
		for (i = 0; i < 3; i++) {
			ss << dis(gen);
		}
		ss << "-";
		for (i = 0; i < 12; i++) {
			ss << dis(gen);
		};
		return ss.str();
	}
}


std::string UniqueId::get()
{
	std::string output = uuid::generate_uuid_v4();
	return output;
}
