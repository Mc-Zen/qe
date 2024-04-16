#pragma once

#include "binary.h"
#include "fmt/core.h"

namespace qe {

constexpr auto format_as(Binary binary) {
	return binary.to_int();
}

}
