
#pragma once
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "binary.h"
#include "binary_phase.h"


namespace qe {

class Pauli {
public:
	explicit Pauli(int num_qubits) : x_vector(num_qubits), z_vector(num_qubits) {}
	explicit Pauli(std::string_view pauli_string) { from_pauli_string(pauli_string); }
	static Pauli Single_X(int num_qubits, int qubit);
	static Pauli Single_Z(int num_qubits, int qubit);

	int num_qubits() const { return static_cast<int>(x_vector.size()); }
	BinaryPhase phase() const { return phase_; }

	/// @brief Check if operator at given qubit has an x component
	int x(int qubit) const { return x_vector[qubit]; }
	/// @brief Check if operator at given qubit has a z component
	int z(int qubit) const { return z_vector[qubit]; };


	/// @brief Check if operator at given qubit has an x component
	void set_x(int qubit, int value) { x_vector[qubit] = value; }
	/// @brief Check if operator at given qubit has a z component
	void set_z(int qubit, int value) { z_vector[qubit] = value; };

	void set_phase(BinaryPhase phase) { phase_ = phase; };

	friend bool operator==(const Pauli& a, const Pauli& b) = default;

	std::string to_string(bool add_phase = true) const;

private:
	void from_pauli_string(std::string_view pauli_string);
	void from_phaseless_pauli_string(std::string_view pauli_string);
	int count_Y_paulis() const;

	std::vector<Binary> x_vector;
	std::vector<Binary> z_vector;
	BinaryPhase phase_;
};


class FPauli {};

} // namespace qe

