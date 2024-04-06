
#pragma once
#include <string>
#include <array>
#include <vector>
#include <map>
#include <cassert>


namespace qe {

	struct GateData {};

	enum class GateType {
		I, X, Y, Z,
		H, S, SDG, SX, SXDG,
		CX, CZ, SWAP,
		Other
	};

	struct Gate {
		int qubit{};
		int target{ -1 };
		GateType type{};
		//std::optional<std::unique_ptr<GateData>> data{};

		friend bool operator==(const Gate& g1, const Gate& g2) = default;
	};

	constexpr bool is_clifford_gate(const Gate& gate) {
		return gate.type >= GateType::I && gate.type <= GateType::SWAP;
	}

	constexpr bool is_pauli_gate(const Gate& gate) {
		return gate.type >= GateType::I && gate.type <= GateType::Z;
	}

	constexpr bool is_two_qubit_gate(const Gate& gate) {
		return gate.type >= GateType::CX && gate.type <= GateType::SWAP;
	}

	constexpr bool is_single_qubit_gate(const Gate& gate) {
		return !is_two_qubit_gate(gate);
	}


	namespace Filter {
		struct no_filter {
			bool operator()(const Gate&) const { return true; }
		};

		struct two_qubit_gate_filter {
			bool operator()(const Gate& gate) const { return is_two_qubit_gate(gate); }
		};
	}


	class Circuit {
	public:
		/// @brief Creates a circuit with the given number of qubits. 
		explicit Circuit(int num_qubits) : num_qubits(num_qubits) {}

		/// @brief Adds a gate to the end of the circuit. 
		void add(const Gate& gate) {
			assert(gate.qubit >= 0 && gate.qubit < num_qubits && "The circuit has not enough qubits for this gate");
			assert(gate.target < num_qubits && "The circuit has not enough qubits for this gate");
			gates.push_back(gate);
		}

		/// @brief Adds a gate to the front of the circuit. 
		void add_front(const Gate& gate) { gates.insert(gates.begin(), gate); }

		/// @brief Appends another circuit to the end of the circuit. 
		void append(const Circuit& other);
		/// @brief Returns a new circuit by concatenating both circuits. 
		Circuit compose(const Circuit& other) const;
		/// @brief Returns the inverted circuit
		Circuit inverse() const;

		Circuit& operator+=(const Circuit& other) { append(other); return *this; }
		Circuit operator+(const Circuit& other) const { return compose(other); }

		void clear() { gates.clear(); }


		/// @brief Computes the depth of the circuit by identifying which operations
		///    can be parallelized. The filter can be used to ignore certain gates. It 
		///    receives each gate of the circuit in turn and should return a boolean. 
		///    Gates for which the filter returns false are ignored in the computation. 
		template<typename F = Filter::no_filter>
		int depth(F filter = {}) const;
		/// @brief Returns the two-qubit gate depth, ignoring all single-qubit gates. 
		int two_qubit_depth() const { return depth<Filter::two_qubit_gate_filter>(); }

		/// @brief Counts each gate. Returns a dictionary with each gate and how often it occurs. 
		std::map<GateType, int> count_ops() const;

		void i(int qubit) { add({ .qubit = qubit, .type = GateType::I }); }
		void x(int qubit) { add({ .qubit = qubit, .type = GateType::X }); }
		void y(int qubit) { add({ .qubit = qubit, .type = GateType::Y }); }
		void z(int qubit) { add({ .qubit = qubit, .type = GateType::Z }); }
		void h(int qubit) { add({ .qubit = qubit, .type = GateType::H }); }
		void s(int qubit) { add({ .qubit = qubit, .type = GateType::S }); }
		void sdg(int qubit) { add({ .qubit = qubit, .type = GateType::SDG }); }
		void sx(int qubit) { add({ .qubit = qubit, .type = GateType::SX }); }
		void sxdg(int qubit) { add({ .qubit = qubit, .type = GateType::SXDG }); }
		void cx(int control, int target) { add({ .qubit = control, .target = target, .type = GateType::CX }); }
		void cz(int control, int target) { add({ .qubit = control, .target = target, .type = GateType::CZ }); }
		void swap(int control, int target) { add({ .qubit = control, .target = target, .type = GateType::SWAP }); }

		/// @brief Generates a visual representation of the circuit and returns it as 
		///    a string for printing. 
		std::string draw() const;

		/// @brief Returns the number of gates in the circuit. 
		size_t size() const { return gates.size(); }

		auto begin() { return gates.begin(); }
		auto begin() const { return gates.begin(); }
		auto cbegin() const { return begin(); }
		auto end() { return gates.end(); }
		auto end() const { return gates.end(); }
		auto cend() const { return end(); }

		auto rbegin() { return gates.rbegin(); }
		auto rbegin() const { return gates.rbegin(); }
		auto crbegin() const { return rbegin(); }
		auto rend() { return gates.rend(); }
		auto rend() const { return gates.rend(); }
		auto crend() const { return rend(); }

	private:
		std::vector<Gate> gates;
		int num_qubits{};
	};

	template<class Filter>
	int Circuit::depth(Filter filter) const {
		std::vector<int> tracks(num_qubits);
		for (const auto& gate : gates) {
			if (!filter(gate)) continue;

			if (is_single_qubit_gate(gate)) {
				++tracks[gate.qubit];
			}
			else {
				auto max_track_length = std::max(tracks[gate.qubit], tracks[gate.target]);
				tracks[gate.qubit] = max_track_length + 1;
				tracks[gate.target] = max_track_length + 1;
			}
		}
		int max_track_length = 0;
		for (auto track : tracks) {
			max_track_length = std::max(track, max_track_length);
		}
		return max_track_length;
	}

}
