#include "pauli.h"
#include <cassert>

using namespace qe;



static qe::Pauli Single_X(int num_qubits, int qubit) {
	Pauli pauli(num_qubits);
	pauli.set_x(qubit, 1);
	return pauli;
}

static qe::Pauli Single_Z(int num_qubits, int qubit) {
	Pauli pauli(num_qubits);
	pauli.set_z(qubit, 1);
	return pauli;
}

void qe::Pauli::from_pauli_string(std::string_view pauli_string) {
	assert(pauli_string.length() > 0 && "A Pauli string cannot be empty");

	int phase_length{ 1 };
	switch (pauli_string[0]) {
	case '+': break;
	case '-': {
		if (pauli_string[1] == 'i') {
			phase_ = 3;
			phase_length = 2;
		} else {
			phase_ = 2;
		}
		break;
	}
	case 'i': phase_ = 1; break;
	default: phase_length = 0; break;
	}
	from_phaseless_pauli_string(pauli_string.substr(phase_length));
	phase_ += count_Y_paulis();
}

void qe::Pauli::from_phaseless_pauli_string(std::string_view pauli_string) {
	auto num_qubits = pauli_string.size();
	x_vector.resize(num_qubits);
	z_vector.resize(num_qubits);
	size_t i{};
	for (char c : pauli_string) {
		switch (c) {
		case 'I': break;
		case 'X': x_vector[i] = 1; break;
		case 'Y':
			x_vector[i] = 1;
			z_vector[i] = 1;
			break;
		case 'Z': z_vector[i] = 1; break;
		default: break;
		}
		++i;
	}
}

int qe::Pauli::count_Y_paulis() const {
	auto n = num_qubits();
	int y_count{};
	for (int qubit = 0; qubit < n; ++qubit) {
		if (x(qubit) && z(qubit)) {
			++y_count;
		}
	}
	return y_count;
}

std::string qe::Pauli::to_string(bool print_phase) const {
	constexpr std::array<char, 4> c{ 'I', 'X', 'Z', 'Y' };
	std::string str;
	if (print_phase) {
		auto printed_phase = phase() - count_Y_paulis();
		str = printed_phase.to_string();
	}
	str.reserve(str.length() + num_qubits());
	for (int i = 0; i < num_qubits(); ++i) {
		str += c[x(i) + z(i) * 2];
	}
	return str;
}