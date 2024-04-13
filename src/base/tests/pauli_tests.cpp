#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"

#include "matrix.h"
#include "pauli.h"
#include <iostream>

using namespace qe;

TEST_CASE("Pauli num_qubits constructor") {
	auto p = Pauli(7);
	REQUIRE(p.num_qubits() == 7);
	for (int i = 0; i < p.num_qubits(); ++i) {
		REQUIRE(p.x(i) == 0);
		REQUIRE(p.z(i) == 0);
	}
}

TEST_CASE("Pauli string constructor") {
	auto p = Pauli("XYZ");
	REQUIRE(p.phase() == 1);

	p = Pauli("iXYZ");
	REQUIRE(p.phase() == 2);

	p = Pauli("-XYZ");
	REQUIRE(p.phase() == 3);

	p = Pauli("-iXYZ");
	REQUIRE(p.phase() == 0);
}

TEST_CASE("Pauli set X/Z") {
	auto p = Pauli("-XYZ");
	p.set_x(0, 0);
	p.set_z(0, 1);
	REQUIRE(p.x(0) == 0);
	REQUIRE(p.z(0) == 1);
}

TEST_CASE("Format Paulis") {
	REQUIRE(std::format("{}", Pauli{ "iXYYZ" }) == "iXYYZ");
	REQUIRE(std::format("{}", Pauli{ "X" }) == "+X");
	REQUIRE(std::format("{}", Pauli{ "+X" }) == "+X");
	REQUIRE(std::format("{}", Pauli{ "iX" }) == "iX");
	REQUIRE(std::format("{}", Pauli{ "-iIX" }) == "-iIX");
	REQUIRE(std::format("{}", Pauli{ "-IY" }) == "-IY");
	REQUIRE(std::format("{}", Pauli{ "-IYY" }) == "-IYY");
}