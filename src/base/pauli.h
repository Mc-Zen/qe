
#pragma once
#include <cstddef>
#include <vector>


namespace qe {

	class Pauli {
	public:
		explicit Pauli(int num_qubits) {}

	private:
		std::vector<std::byte> x;
		std::vector<std::byte> z;
	};

}
