
#pragma once
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "binary.h"
#include "binary_phase.h"


namespace qe {

/// @brief N-qubit Pauli operator in the form $i^q X^r Z^s$ with $q \in {0,1,2,3}$ and
///        binary vectors $r,s \in \mathbb{F}_2^n$.
class Pauli {
public:

	/// @brief Constructs an all-identity Pauli operator with the given number of qubits.
	/// @param num_qubits Number of qubits
	explicit Pauli(int num_qubits) : x_vector(num_qubits), z_vector(num_qubits) {}

	/// @brief Constructs a Pauli operator from a Pauli string, e.g., "XYZ" or "-iXI".
	///        Note that the phase is given through the form above and since $Y = iXZ$, the
	///        phase will be $i^2 = -1$ for "iY" instead of $i$ (as an example).
	///        The first Pauli character in the Pauli string corresponds to the first qubit. 
	/// @param pauli_string Pauli string. Can contain a phase "", "+", "-", "i", or "-i"
	///                     followed by a series of Pauli characters "I", "X", "Y", and "Z".
	explicit Pauli(std::string_view pauli_string) { from_pauli_string(pauli_string); }


	/// @brief Constructs a Pauli operator with identity on all qubits except for one $X$ at
	///        the specified qubit.
	static Pauli Single_X(int num_qubits, int qubit);

	/// @brief Constructs a Pauli operator with identity on all qubits except for one $Z$ at
	///        the specified qubit.
	static Pauli Single_Z(int num_qubits, int qubit);

	/// @brief Constructs a Pauli operator with identity on all qubits except for one $Z$ at
	///        the specified qubit.
	static Pauli Single_Y(int num_qubits, int qubit);

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

	/// @brief Sets the phase of the Pauli operator.
	/// @param phase
	void set_phase(BinaryPhase phase) { phase_ = phase; };

	friend bool operator==(const Pauli& a, const Pauli& b) = default;

	/// @brief Prints the Pauli operator to a string. 
	///        The first Pauli character in the Pauli string corresponds to the first qubit. 
	/// @param add_phase When set to false, the phase is omitted. 
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
