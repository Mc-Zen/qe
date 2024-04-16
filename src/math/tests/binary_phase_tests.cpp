#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"


#include "binary_phase.h"
#include "format_binary_phase.h"

using namespace qe;


TEST_CASE("BinaryPhase constructor") {
	REQUIRE(BinaryPhase{ 0 }.to_int() == 0);
	REQUIRE(BinaryPhase{ 1 }.to_int() == 1);
	REQUIRE(BinaryPhase{ 2 }.to_int() == 2);
	REQUIRE(BinaryPhase{ 3 }.to_int() == 3);
	REQUIRE(BinaryPhase{ -1 }.to_int() == 3);
	REQUIRE(BinaryPhase{ 10 }.to_int() == 2);
}

TEST_CASE("BinaryPhase arithmetic") {
	REQUIRE(BinaryPhase{ 0 } + BinaryPhase{ 0 } == BinaryPhase{ 0 });
	REQUIRE(BinaryPhase{ 0 } + BinaryPhase{ 1 } == BinaryPhase{ 1 });
	REQUIRE(BinaryPhase{ 3 } + BinaryPhase{ 0 } == BinaryPhase{ 3 });
	REQUIRE(BinaryPhase{ 1 } + BinaryPhase{ 3 } == BinaryPhase{ 0 });

	REQUIRE(++BinaryPhase{ 3 } == BinaryPhase{ 0 });
	REQUIRE(--BinaryPhase{ 0 } == BinaryPhase{ 3 });

	REQUIRE(BinaryPhase{ 0 }.is_real());
	REQUIRE(BinaryPhase{ 2 }.is_real());
	REQUIRE(!BinaryPhase{ 1 }.is_real());
	REQUIRE(!BinaryPhase{ 3 }.is_real());

	BinaryPhase phase{ 2 };
	phase += 3;
	REQUIRE(phase == BinaryPhase{ 1 });
}

TEST_CASE("Format BinaryPhase") {
	REQUIRE(fmt::format("{}", BinaryPhase{ 0 }) == "+");
	REQUIRE(fmt::format("{}", BinaryPhase{ 1 }) == "i");
	REQUIRE(fmt::format("{}", BinaryPhase{ 2 }) == "-");
	REQUIRE(fmt::format("{}", BinaryPhase{ 3 }) == "-i");
}