#pragma once

#include <Core/Math/Math.hpp>
#include <Core/Math/Vector3.hpp>
#include <Core/Math/Vector4.hpp>

inline void Vector3ContentCheck(const SW::Vector3<f32>& vector, f32 x, f32 y, f32 z) {
	CHECK(vector.x == x);
	CHECK(vector.r == x);
	CHECK(vector.s == x);
	CHECK(vector.u == x);

	CHECK(vector.y == y);
	CHECK(vector.g == y);
	CHECK(vector.t == y);
	CHECK(vector.v == y);

	CHECK(vector.z == z);
	CHECK(vector.b == z);
	CHECK(vector.p == z);
	CHECK(vector.w == z);
}

TEST_CASE("Vector3 - constructors - tests") {

	SUBCASE("Vector3()") {
		SW::Vector3<f32> vector;

		Vector3ContentCheck(vector, 0.f, 0.f, 0.f);
	}

	SUBCASE("Vector3(T x)") {
		SW::Vector3<f32> vector = { 2.f };

		Vector3ContentCheck(vector, 2.f, 2.f, 2.f);
	}

	SUBCASE("Vector3(T x, T y, T z)") {
		SW::Vector3<f32> vector = { 2.f, -5.f, 8.f };

		Vector3ContentCheck(vector, 2.f, -5.f, 8.f);
	}

	SUBCASE("Vector3(std::initializer_list<T> values)") {
		SW::Vector3<f32> vector1({ 2.f });
		SW::Vector3<f32> vector2({ 2.f, -5.f });
		SW::Vector3<f32> vector3({ 2.f, -5.f, 6.f, 8.f });

		Vector3ContentCheck(vector1, 2.f, 2.f, 2.f);
		Vector3ContentCheck(vector2, 2.f, -5.f, -5.f);
		Vector3ContentCheck(vector3, 2.f, -5.f, 6.f);
	}
}

TEST_CASE("Vector3 - ToString - tests") {
	SW::Vector3<f32> vector1 = { 2.f, -5.f, 9.f };
	CHECK(vector1.ToString() == "{ 2.000000, -5.000000, 9.000000 }");

	SW::Vector3<f32> vector2 = { 2.123456789f, -5.987654321f, 12.999999f };
	CHECK(vector2.ToString() == "{ 2.123457, -5.987654, 12.999999 }");
}

TEST_CASE("Vector3 - T* ValuePtr - tests") {
	SW::Vector3<f32> vector = { 2.f, -5.f, 9.f };

	CHECK(vector.ValuePtr() == &vector.x);
	CHECK(vector.ValuePtr() + 1 == &vector.y);
	CHECK(vector.ValuePtr() + 2 == &vector.z);
}

TEST_CASE("Vector3 - T Length - tests")
{
	SW::Vector3<f32> vector = { 2.f, -5.f, 9.f };

	CHECK(vector.Length() > 10.3923f);
	CHECK(vector.Length() < 10.4924f);
}

TEST_CASE("Vector3 - void Normalize - tests")
{
	SW::Vector3<f32> vector = { 2.f, -5.f, 9.f };

	vector.Normalize();

	CHECK(vector.Length() > 0.9999f);
	CHECK(vector.Length() < 1.0001f);

	SW::Vector3<f32> vector2 = { 0.f, 0.f, 0.f };
	vector2.Normalize();

	CHECK(vector2.x == 0.f);
	CHECK(vector2.y == 0.f);
	CHECK(vector2.z == 0.f);
}

TEST_CASE("Vector3 - void Normalized - tests")
{
	SW::Vector3<f32> vector = { 2.f, -5.f, 9.f };

	SW::Vector3<f32> normalizedVector = vector.Normalized();

	CHECK(normalizedVector.Length() > 0.9999f);
	CHECK(normalizedVector.Length() < 1.0001f);

	SW::Vector3<f32> vector2 = { 0.f, 0.f, 0.f };
	SW::Vector3<f32> normalizedVector2 = vector2.Normalized();

	CHECK(normalizedVector2.x == 0.f);
	CHECK(normalizedVector2.y == 0.f);
	CHECK(normalizedVector2.z == 0.f);
}

TEST_CASE("Vector3 - T Dot - tests")
{
	SW::Vector3<f32> vector1 = { 2.f, -5.f, 9.f };
	SW::Vector3<f32> vector2 = { 3.f, 4.f, -8.f };

	CHECK(vector1.Dot(vector2) == -86.f);
	CHECK(vector2.Dot(vector1) == -86.f);
}

TEST_CASE("Vector3 - Vector3<T> Cross(const Vector3<T>& other) - tests")
{
	SW::Vector3<f32> vector1 = { 2.f, -5.f, 9.f };
	SW::Vector3<f32> vector2 = { 3.f, 4.f, -8.f };

	SW::Vector3<f32> crossVector1 = vector1.Cross(vector2);
	SW::Vector3<f32> crossVector2 = vector2.Cross(vector1);

	CHECK(crossVector1.x == 4.f);
	CHECK(crossVector1.y == 43.f);
	CHECK(crossVector1.z == 23.f);

	CHECK(crossVector2.x == -4.f);
	CHECK(crossVector2.y == -43.f);
	CHECK(crossVector2.z == -23.f);
}

TEST_CASE("Vector3 - T AngleBetween(const Vector3<T>& other) - tests")
{
	SW::Vector3<f32> vector1 = { 1.f, 0.f, 0.f };
	SW::Vector3<f32> vector2 = { 0.f, 1.f, 0.f };

	CHECK(SW::Math::ToDegrees(vector1.AngleBetween(vector2)) == 90.f);
	CHECK(SW::Math::ToDegrees(vector2.AngleBetween(vector1)) == 90.f);
}

TEST_CASE("Vector3 - T DistanceBetween(const Vector3<T>& other) - tests")
{
	SW::Vector3<f32> vector1 = { 1.f, 1.f, 1.f };
	SW::Vector3<f32> vector2 = { 2.f, 2.f, 2.f };

	CHECK(vector1.DistanceBetween(vector2) > 1.73f);
	CHECK(vector1.DistanceBetween(vector2) < 1.74f);

	CHECK(vector2.DistanceBetween(vector1) > 1.73f);
	CHECK(vector2.DistanceBetween(vector1) < 1.74f);
}

TEST_CASE("Vector3 - static Vector3<T> FromVector4(const Vector4<T>& other) - tests")
{
	SW::Vector4<f32> vector4 = { 1.f, 2.f, 3.f, 4.f };

	SW::Vector3<f32> vector3 = SW::Vector3<f32>::FromVector4(vector4);

	CHECK(vector3.x == 1.f);
	CHECK(vector3.y == 2.f);
	CHECK(vector3.z == 3.f);
}

TEST_CASE("Vector3 - Vector3 to Vector3 operators - tests")
{
	SUBCASE("Vector3 += Vector3")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 1.f };
		SW::Vector3<f32> vector2 = { 3.f, 4.f, 4.f };

		vector1 += vector2;

		Vector3ContentCheck(vector1, 5.f, -1.f, 5.f);
	}

	SUBCASE("Vector3 -= Vector3")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 5.f };
		SW::Vector3<f32> vector2 = { 3.f, 4.f, 2.f };

		vector1 -= vector2;

		Vector3ContentCheck(vector1, -1.f, -9.f, 3.f);
	}

	SUBCASE("Vector3 *= Vector3")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 2.f };
		SW::Vector3<f32> vector2 = { 3.f, 4.f, 2.f };

		vector1 *= vector2;

		Vector3ContentCheck(vector1, 6.f, -20.f, 4.f);
	}

	SUBCASE("Vector3 /= Vector3")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 2.f };
		SW::Vector3<f32> vector2 = { 3.f, 4.f, 2.f };

		vector1 /= vector2;

		Vector3ContentCheck(vector1, 0.66666669f, -1.25f, 1.f);

		SW::Vector3<f32> vector3 = { 0.f, 0.f, 0.f };
		SW::Vector3<f32> vector4 = { 3.f, 4.f, 5.f };

		vector4 /= vector3;

		Vector3ContentCheck(vector4, 3.f, 4.f, 5.f);
	}

	SUBCASE("Vector3 == Vector3")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 6.f };
		SW::Vector3<f32> vector2 = { 2.f, -5.f, 6.f };

		CHECK(vector1 == vector2);
	}

	SUBCASE("Vector3 != Vector3")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 6.f };
		SW::Vector3<f32> vector2 = { 3.f, 4.f, 7.f };

		CHECK(vector1 != vector2);
	}

	SUBCASE("Vector3 + Vector3")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 2.f };
		SW::Vector3<f32> vector2 = { 3.f, 4.f, 2.f };

		SW::Vector3<f32> vector3 = vector1 + vector2;

		Vector3ContentCheck(vector3, 5.f, -1.f, 4.f);
	}

	SUBCASE("Vector3 - Vector3")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 3.f };
		SW::Vector3<f32> vector2 = { 3.f, 4.f, 2.f };

		SW::Vector3<f32> vector3 = vector1 - vector2;

		Vector3ContentCheck(vector3, -1.f, -9.f, 1.f);
	}

	SUBCASE("Vector3 * Vector3")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 2.f };
		SW::Vector3<f32> vector2 = { 3.f, 4.f, 2.f };

		SW::Vector3<f32> vector3 = vector1 * vector2;

		Vector3ContentCheck(vector3, 6.f, -20.f, 4.f);
	}

	SUBCASE("Vector3 / Vector3")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 2.f };
		SW::Vector3<f32> vector2 = { 3.f, 4.f, 2.f };

		SW::Vector3<f32> vector3 = vector1 / vector2;

		Vector3ContentCheck(vector3, 0.66666669f, -1.25f, 1.f);

		SW::Vector3<f32> vector4 = { 0.f, 0.f, 0.f };
		SW::Vector3<f32> vector5 = { 3.f, 4.f, 5.f };

		SW::Vector3<f32> vector6 = vector5 / vector4;

		Vector3ContentCheck(vector6, 3.f, 4.f, 5.f);
	}
}

TEST_CASE("Vector3 - Vector3 to scalar operators - tests")
{
	SUBCASE("Vector3 += T")
	{
		SW::Vector3<f32> vector = { 2.f, -5.f, 1.f };

		vector += 3.f;

		Vector3ContentCheck(vector, 5.f, -2.f, 4.f);
	}

	SUBCASE("Vector3 -= T")
	{
		SW::Vector3<f32> vector = { 2.f, -5.f, 5.f };

		vector -= 3.f;

		Vector3ContentCheck(vector, -1.f, -8.f, 2.f);
	}

	SUBCASE("Vector3 *= T")
	{
		SW::Vector3<f32> vector = { 2.f, -5.f, 2.f };

		vector *= 3.f;

		Vector3ContentCheck(vector, 6.f, -15.f, 6.f);
	}

	SUBCASE("Vector3 /= T")
	{
		SW::Vector3<f32> vector = { 2.f, -4.f, 2.f };

		vector /= 2.f;

		Vector3ContentCheck(vector, 1.f, -2.f, 1.f);

		SW::Vector3<f32> vector2 = { 0.f, 0.f, 0.f };

		vector2 /= 3.f;

		Vector3ContentCheck(vector2, 0.f, 0.f, 0.f);
	}

	SUBCASE("Vector3 + T")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 2.f };

		SW::Vector3<f32> vector2 = vector1 + 3.f;

		Vector3ContentCheck(vector2, 5.f, -2.f, 5.f);
	}

	SUBCASE("Vector3 - T")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 2.f };

		SW::Vector3<f32> vector2 = vector1 - 3.f;

		Vector3ContentCheck(vector2, -1.f, -8.f, -1.f);
	}

	SUBCASE("Vector3 * T")
	{
		SW::Vector3<f32> vector1 = { 2.f, -5.f, 2.f };
		SW::Vector3<f32> vector2 = vector1 * 3.f;
		
		Vector3ContentCheck(vector2, 6.f, -15.f, 6.f);

		SW::Vector3<f32> vector3 = { 0.f, 0.f, 0.f };
		SW::Vector3<f32> vector4 = vector3 * 3.f;

		Vector3ContentCheck(vector4, 0.f, 0.f, 0.f);
	}

	SUBCASE("Vector3 / T")
	{
		SW::Vector3<f32> vector1 = { 2.f, -2.f, 2.f };
		SW::Vector3<f32> vector2 = vector1 / 2.f;

		Vector3ContentCheck(vector2, 1.f, -1.f, 1.f);

		SW::Vector3<f32> vector3 = { 0.f, 0.f, 0.f };
		SW::Vector3<f32> vector4 = vector3 / 3.f;

		Vector3ContentCheck(vector4, 0.f, 0.f, 0.f);
	}
}
