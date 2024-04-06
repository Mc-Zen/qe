#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"

#include "circuit.h"
#include <iostream>

using namespace qe;


TEST_CASE("Circuit constructor") {
	auto qc = Circuit(4);
	qc.h(0);
	qc.cx(0, 1);
	qc.cx(0, 2);
	qc.cx(0, 3);
	qc.s(1);
	qc.x(3);
	//qc.add(Gate{ .qubit = 0, .target =1, .type = GateType::Other });
	//qc.add(Gate{ .qubit = 2, .target=3, .type = GateType::Other });
	//qc.h(1);
	qc.swap(1, 3);
	std::cout << qc.draw();
	std::cout << qc.inverse().draw();

	auto qc2 = Circuit(4);
	qc2.cx(0, 3);
	qc2.x(3);
	qc += qc2;
	
	auto cops = qc.count_ops();
	qc.h(2);
	qc.s(2);
	qc.cx(1, 2);
	std::cout << qc.draw();
	std::cout << qc.depth() << " " << qc.two_qubit_depth();
	//std::cout << cops[GateType::CZ];
}