#pragma once

#include <Core/Math/Vector4.hpp>

inline void Vector4ContentCheck(const SW::Vector4<f32>& vector, f32 x, f32 y, f32 z, f32 w) {
	CHECK(vector.x == x);
	CHECK(vector.r == x);

	CHECK(vector.y == y);
	CHECK(vector.g == y);

	CHECK(vector.z == z);
	CHECK(vector.b == z);

	CHECK(vector.w == w);
	CHECK(vector.a == w);
}

TEST_CASE("Vector4 - constructors - tests") {

	SUBCASE("Vector4()") {
		SW::Vector4<f32> vector;

		Vector4ContentCheck(vector, 0.f, 0.f, 0.f, 0.f);
	}

	SUBCASE("Vector4(T x)") {
		SW::Vector4<f32> vector = { 2.f };

		Vector4ContentCheck(vector, 2.f, 2.f, 2.f, 2.f);
	}

	SUBCASE("Vector4(T x, T y, T z)") {
		SW::Vector4<f32> vector = { 2.f, -5.f, 8.f, 3.f };

		Vector4ContentCheck(vector, 2.f, -5.f, 8.f, 3.f);
	}

	SUBCASE("Vector4(std::initializer_list<T> values)") {
		SW::Vector4<f32> vector1({ 2.f });
		SW::Vector4<f32> vector2({ 2.f, -5.f });
		SW::Vector4<f32> vector3({ 2.f, -5.f, 6.f, 8.f });

		Vector4ContentCheck(vector1, 2.f, 2.f, 2.f, 2.f);
		Vector4ContentCheck(vector2, 2.f, -5.f, -5.f, -5.f);
		Vector4ContentCheck(vector3, 2.f, -5.f, 6.f, 8.f);
	}
}

TEST_CASE("Vector4 - ToString - tests") {
	SW::Vector4<f32> vector1 = { 2.f, -5.f, 9.f, 2.f };
	CHECK(vector1.ToString() == "{ 2.000000, -5.000000, 9.000000, 2.000000 }");

	SW::Vector4<f32> vector2 = { 2.123456789f, -5.987654321f, 12.999999f, 12.999999f };
	CHECK(vector2.ToString() == "{ 2.123457, -5.987654, 12.999999, 12.999999 }");
}

TEST_CASE("Vector4 - T* ValuePtr - tests") {
	SW::Vector4<f32> vector = { 2.f, -5.f, 9.f, 3.f };

	CHECK(vector.ValuePtr() == &vector.x);
	CHECK(vector.ValuePtr() + 1 == &vector.y);
	CHECK(vector.ValuePtr() + 2 == &vector.z);
	CHECK(vector.ValuePtr() + 3 == &vector.w);
}

TEST_CASE("Vector4 - Vector4 to scalar operators - tests")
{
	SW::Vector4<f32> vector = { 2.f, -5.f, 9.f, 3.f };

	SUBCASE("Vector4 + T")
	{
		SW::Vector4<f32> result = vector + 2.f;

		CHECK(result.x == 4.f);
		CHECK(result.y == -3.f);
		CHECK(result.z == 11.f);
		CHECK(result.w == 5.f);
	}

	SUBCASE("Vector4 - T")
	{
		SW::Vector4<f32> result = vector - 2.f;

		CHECK(result.x == 0.f);
		CHECK(result.y == -7.f);
		CHECK(result.z == 7.f);
		CHECK(result.w == 1.f);
	}

	SUBCASE("Vector4 * T")
	{
		SW::Vector4<f32> result = vector * 2.f;

		CHECK(result.x == 4.f);
		CHECK(result.y == -10.f);
		CHECK(result.z == 18.f);
		CHECK(result.w == 6.f);
	}

	SUBCASE("Vector4 / T")
	{
		SW::Vector4<f32> result = vector / 2.f;

		CHECK(result.x == 1.f);
		CHECK(result.y == -2.5f);
		CHECK(result.z == 4.5f);
		CHECK(result.w == 1.5f);
	}
}

TEST_CASE("Vector4 - Vector4 to Vector4 operators - tests")
{
	SW::Vector4<f32> vector1 = { 2.f, -5.f, 9.f, 3.f };
	SW::Vector4<f32> vector2 = { 3.f, 4.f, -8.f, 2.f };

	SUBCASE("Vector4 + Vector4")
	{
		SW::Vector4<f32> result = vector1 + vector2;

		CHECK(result.x == 5.f);
		CHECK(result.y == -1.f);
		CHECK(result.z == 1.f);
		CHECK(result.w == 5.f);
	}

	SUBCASE("Vector4 - Vector4")
	{
		SW::Vector4<f32> result = vector1 - vector2;

		CHECK(result.x == -1.f);
		CHECK(result.y == -9.f);
		CHECK(result.z == 17.f);
		CHECK(result.w == 1.f);
	}

	SUBCASE("Vector4 * Vector4")
	{
		SW::Vector4<f32> result = vector1 * vector2;

		CHECK(result.x == 6.f);
		CHECK(result.y == -20.f);
		CHECK(result.z == -72.f);
		CHECK(result.w == 6.f);
	}

	SUBCASE("Vector4 / Vector4")
	{
		SW::Vector4<f32> result = vector1 / vector2;

		CHECK(result.x == 2.f / 3.f);
		CHECK(result.y == -5.f / 4.f);
		CHECK(result.z == -9.f / 8.f);
		CHECK(result.w == 3.f / 2.f);
	}
}
