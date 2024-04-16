#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"


#include "binary.h"
#include "format_binary.h"

using namespace qe;


TEST_CASE("Binary constructor") {
	REQUIRE(Binary{ 0 }.to_int() == 0);
	REQUIRE(Binary{ 1 }.to_int() == 1);
}

TEST_CASE("Binary arithmetic") {
	REQUIRE(Binary{ 0 } + Binary{ 0 } == Binary{ 0 });
	REQUIRE(Binary{ 0 } + Binary{ 1 } == Binary{ 1 });
	REQUIRE(Binary{ 1 } + Binary{ 0 } == Binary{ 1 });
	REQUIRE(Binary{ 1 } + Binary{ 1 } == Binary{ 0 });

	REQUIRE(Binary{ 0 } * Binary{ 0 } == Binary{ 0 });
	REQUIRE(Binary{ 0 } * Binary{ 1 } == Binary{ 0 });
	REQUIRE(Binary{ 1 } * Binary{ 0 } == Binary{ 0 });
	REQUIRE(Binary{ 1 } * Binary{ 1 } == Binary{ 1 });

	REQUIRE(Binary{ 0 }.negate() == Binary{ 1 });
	REQUIRE(Binary{ 1 }.negate() == Binary{ 0 });

	REQUIRE(~Binary{ 0 } == Binary{ 1 });
	REQUIRE(~Binary{ 1 } == Binary{ 0 });
}

TEST_CASE("Format Binary") {
	REQUIRE(fmt::format("{}", Binary{ 0 }) == "0");
	REQUIRE(fmt::format("{}", Binary{ 1 }) == "1");
}
