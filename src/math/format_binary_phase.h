#pragma once

#include "binary_phase.h"
#include "fmt/format.h"

namespace qe {

inline auto format_as(BinaryPhase binary_phase) {
	static constexpr std::string_view values[4] = { "+", "i", "-", "-i" };
	return values[binary_phase.to_int()];
}

}
