#pragma once

namespace qe {


	/// @brief Binary number with modulo 2 arithmetic, 1+1=0.
	class Binary {
	public:
		constexpr Binary() = default;
		explicit(false) constexpr Binary(bool value) : value(value) {};
		explicit constexpr Binary(int value) : value(value & 1) {};

		constexpr Binary& operator+=(const Binary& a) { value ^= a.value; return *this; }
		constexpr Binary& operator-=(const Binary& a) { return this->operator+=(a); }
		constexpr Binary& operator*=(const Binary& a) { value &= a.value; return *this; }
		constexpr Binary& operator&=(const Binary& a) { value &= a.value; return *this; }
		constexpr Binary& operator|=(const Binary& a) { value |= a.value; return *this; }
		constexpr friend Binary operator+(const Binary& a, const Binary& b) { return Binary{ a } += b; }
		constexpr friend Binary operator-(const Binary& a, const Binary& b) { return Binary{ a } -= b; }
		constexpr friend Binary operator*(const Binary& a, const Binary& b) { return Binary{ a } *= b; }
		constexpr friend Binary operator&(const Binary& a, const Binary& b) { return Binary{ a } &= b; }
		constexpr friend Binary operator|(const Binary& a, const Binary& b) { return Binary{ a } |= b; }

		explicit constexpr operator int() const { return value; }
		explicit constexpr operator bool() const { return value == 1; }
		constexpr int to_int() const { return value; }
		constexpr Binary& negate() { value = 1 - value; return *this; }
		constexpr Binary operator~() const { return Binary{ !value }; }

		constexpr friend bool operator==(const Binary& a, const Binary& b) = default;

	private:
		int value{};
	};
}
