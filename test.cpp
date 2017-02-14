#include "ChaNGa-acc.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <cstring>

template<typename T>
void test() {
	char const *filename = "test.tipsy";

	constexpr size_t num_particles = 150000;
	std::vector<T> acc(3 * num_particles);
	std::iota(acc.begin(), acc.end(), 0.0); // unique values
	try {
		ChaNGa::write_accelerations(filename, acc.data(), acc.size());
	} catch (ChaNGa::cio_error &e) {
		std::cerr << "I/O error: " << std::strerror(e.error) << '\n';
		return;
	} catch (...) {
		std::cerr << "read_accelerations XDR read error\n";
		return;
	}

	std::vector<T> read_acc(acc.size());
	std::generate(read_acc.begin(), read_acc.end(), []() {return 9e36;});
	try {
		ChaNGa::read_accelerations(filename, read_acc.data(), read_acc.size());
	} catch (ChaNGa::cio_error &e) {
		std::cerr << "I/O error: " << std::strerror(e.error) << '\n';
		return;
	} catch (...) {
		std::cerr << "read_accelerations XDR read error\n";
		return;
	}

	auto last = std::mismatch(acc.begin(), acc.end(), read_acc.begin(),
			[](T a, T b) {return std::abs(a-b) <= 1e-12;});
	if (last.first != acc.end() && last.second != read_acc.end()) {
		std::cerr << "error during read at position {" << std::distance(acc.begin(), last.first) << ", "
				<< std::distance(read_acc.begin(), last.second) << "}\n" << *(last.second) << " should be " << *(last.first) << '\n';
	} else {
		std::cout << "Passed\n";
		std::remove(filename);
	}
}

int main() {
	std::cout << "Running test<float>(): ";
	test<float>();
	std::cout << "Running test<double>(): ";
	test<double>();
}
