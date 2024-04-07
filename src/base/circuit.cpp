#include "circuit.h"
#include <algorithm>
#include <iterator>

using namespace qe;

auto get_gate_symbol(const auto& gate) {
	switch (gate.type) {
		using enum GateType;
	case I: return "I";
	case X: return "X";
	case Y: return "Y";
	case Z: return "Z";
	case H: return "H";
	case S: return "S";
	case SDG: return "SDG";
	case SX: return "SX";
	case SXDG: return "SXDG";
	case CX: return "o";
	case CZ: return "o";
	case SWAP: return "x";
	default: return "";
	}
}


void qe::Circuit::append(const Circuit& other) {
	std::copy(other.gates.begin(), other.gates.end(), std::back_inserter(gates));
}

Circuit qe::Circuit::compose(const Circuit& other) const {
	Circuit result = *this;
	std::copy(other.gates.begin(), other.gates.end(), std::back_inserter(result.gates));
	return result;
}

Circuit qe::Circuit::inverse() const {
	Circuit result(num_qubits);
	for (auto it = gates.rbegin(); it != gates.rend(); ++it) {
		auto inverse_gate = *it;
		switch (it->type) {
			using enum GateType;
		case SDG: inverse_gate.type = GateType::S; break;
		case S: inverse_gate.type = GateType::SDG; break;
		case SXDG: inverse_gate.type = GateType::SX; break;
		case SX: inverse_gate.type = GateType::SXDG; break;
		case Other: assert(false);
		default: break;
		}
		result.add(inverse_gate);
	}
	return result;
}

std::map<GateType, int> qe::Circuit::count_ops() const {
	std::map<GateType, int> counted_gates;
	for (const auto& gate : gates) {
		++counted_gates[gate.type];
	}
	return counted_gates;
}

std::string qe::Circuit::draw() const {
	std::vector<std::string> tracks(2 * num_qubits - 1);
	auto get_track_index = [](int qubit) { return 2 * qubit; };
	auto get_track = [&](int qubit) -> std::string& { return tracks[get_track_index(qubit)]; };

	auto add_wire_to_all = [&]() {
		for (int qubit = 0; qubit < num_qubits; ++qubit) {
			get_track(qubit) += '-';
		}
	};
	add_wire_to_all();

	for (const auto& gate : gates) {
		auto q1 = gate.qubit;
		auto q2 = gate.target == -1 ? q1 : gate.target;
		if (q1 > q2) std::swap(q1, q2);
		auto t1 = get_track_index(q1);
		auto t2 = get_track_index(q2);


		auto max_track_len = std::max_element(tracks.begin() + t1, tracks.begin() + t2 + 1, [](const auto& a, const auto& b) { return a.size() < b.size(); })->length();

		for (int track = t1; track <= t2; ++track) {
			tracks[track] += std::string(max_track_len - tracks[track].length(), track & 1 ? ' ' : '-');
		}
		if (is_clifford_gate(gate)) {
			get_track(gate.qubit) += get_gate_symbol(gate);
			get_track(gate.qubit) += "-";

			if (is_two_qubit_gate(gate)) {
				switch (gate.type) {
					using enum GateType;
				case CX: get_track(gate.target) += "+"; break;
				case CZ: get_track(gate.target) += "o"; break;
				case SWAP: get_track(gate.target) += "x"; break;
				default: break;
				}
				get_track(gate.target) += "-";
				for (int i = t1 + 1; i < t2; ++i) {
					tracks[i] += "|";
					tracks[i] += i & 1 ? ' ' : '-';
				}
			}
		} else if (gate.type == GateType::Other) {

			tracks[t1] += "|==|";
			for (int i = t1 + 1; i < t2; ++i) {
				tracks[i] += "|  |";
				tracks[i] += i & 1 ? ' ' : '-';
			}
			if (t1 != t2) {
				tracks[t2] += "|__|";
			}
		}
	}



	auto max_track_len = std::max_element(tracks.begin(), tracks.end(), [](const auto& a, const auto& b) {
		return a.length() < b.length();
	})->length();

	for (int qubit = 0; qubit < num_qubits; ++qubit) {
		auto& track = get_track(qubit);
		track += std::string(max_track_len - track.length(), '-');
	}


	std::string result;
	for (const auto& track : tracks) {
		result += track;
		result += '\n';
	}
	return result;
}
