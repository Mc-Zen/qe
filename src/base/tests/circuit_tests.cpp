#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"

#include "circuit.h"
#include <iostream>

using namespace qe;


TEST_CASE("Circuit constructor") { auto qc = Circuit(4); }

TEST_CASE("Add gates") {
	auto qc = Circuit(4);
	qc.h(0);
	qc.cx(0, 1);
	qc.cx(0, 2);
	qc.cx(0, 3);
	qc.s(1);
	qc.x(3);
	REQUIRE(qc.size() == 6);


	auto cops = qc.count_ops();
	REQUIRE(cops[GateType::H] == 1);
	REQUIRE(cops[GateType::S] == 1);
	REQUIRE(cops[GateType::X] == 1);
	REQUIRE(cops[GateType::CX] == 3);
	REQUIRE(cops[GateType::CZ] == 0);
	REQUIRE(cops[GateType::SWAP] == 0);
	REQUIRE(cops[GateType::SX] == 0);

	qc.add_front(Gate{ .type = GateType::X });

	REQUIRE(qc.begin()->type == GateType::X);

	qc.clear();
	REQUIRE(qc.size() == 0);
}

TEST_CASE("Circuit composition") {
	Circuit qc1{ 3 };
	Circuit qc2{ 3 };

	qc1.x(1);
	qc2.y(1);
	auto qc = qc1 + qc2;
	REQUIRE(qc.size() == 2);
	REQUIRE(qc.begin()->type == GateType::X);
	REQUIRE((qc.begin() + 1)->type == GateType::Y);
}

TEST_CASE("Circuit depth") {

	auto qc = Circuit(4);
	qc.h(0);
	qc.cx(0, 1);
	qc.cx(0, 2);
	qc.cx(0, 3);
	qc.s(1);
	qc.x(3);
	qc.swap(1, 3);

	qc.h(2);
	qc.s(2);
	qc.cx(1, 2);
	qc.swap(0, 3);
	// std::cout << qc.draw();
	REQUIRE(qc.depth() == 7);
	REQUIRE(qc.two_qubit_depth() == 5);
}