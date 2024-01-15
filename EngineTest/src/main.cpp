#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <Core/Defines.hpp>
#include <Core/Math/Vector2.hpp>

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

TEST_CASE("testing the factorial function") {
	CHECK(factorial(1) == 1);
	CHECK(factorial(2) == 2);
	CHECK(factorial(3) == 6);
	CHECK(factorial(10) == 3628800);
}

TEST_CASE("Vector2 tests") {
	SW::Vector2<f32> vector = { 1.f };

	CHECK(vector.x == 1.f);
}
