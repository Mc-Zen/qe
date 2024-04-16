#pragma once

#include "matrix.h"
#include "fmt/format.h"


template <class T, qe::Index m, qe::Index n>
struct fmt::formatter<qe::Matrix<T, m, n>> : nested_formatter<T> {
	using Parent = nested_formatter<T>;

	constexpr auto format(const qe::Matrix<T, m, n>& matrix, format_context& ctx) const {
		auto buffer = std::string{};
		std::vector<size_t> colWidths(matrix.cols());
		for (size_t j = 0; j < matrix.cols(); ++j) {
			colWidths[j] = std::accumulate(matrix.col_begin(j), matrix.col_end(j), size_t{ 0 },
				[&](const auto& max, const auto& el) {
					buffer.clear();
					fmt::format_to(std::back_inserter(buffer), "{}", Parent::nested(el));
					return std::max(max, buffer.size());
				});
		}
		for (size_t i = 0; i < matrix.rows(); ++i) {
			fmt::format_to(ctx.out(), "| ");
			for (size_t j = 0; j < matrix.cols(); ++j) {
				buffer.clear();
				fmt::format_to(std::back_inserter(buffer), "{}", Parent::nested(matrix(i, j)));
				fmt::format_to(ctx.out(), "{:{}} ", buffer, colWidths[j]);
			}
			fmt::format_to(ctx.out(), "|\n");
		}
		return ctx.out();
	}
};
