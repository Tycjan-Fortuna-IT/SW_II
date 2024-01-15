#pragma once

#include <Core/Defines.hpp>
#include <Core/Math/Vector2.hpp>


void Vector2ContentCheck(const SW::Vector2<f32>& vector, f32 x, f32 y)
{
	CHECK(vector.x == x);
	CHECK(vector.r == x);
	CHECK(vector.s == x);
	CHECK(vector.u == x);

	CHECK(vector.y == y);
	CHECK(vector.g == y);
	CHECK(vector.t == y);
	CHECK(vector.v == y);
}

TEST_CASE("Vector2 - constructors - tests")
{

	SUBCASE("Vector2()")
	{
		SW::Vector2<f32> vector;

		Vector2ContentCheck(vector, 0.f, 0.f);
	}

	SUBCASE("Vector2(T x)")
	{
		SW::Vector2<f32> vector = { 2.f };

		Vector2ContentCheck(vector, 2.f, 2.f);
	}

	SUBCASE("Vector2(T x, T y)")
	{
		SW::Vector2<f32> vector = { 2.f, -5.f };

		Vector2ContentCheck(vector, 2.f, -5.f);
	}

}

TEST_CASE("Vector2 - ToString - tests")
{
	SW::Vector2<f32> vector1 = { 2.f, -5.f };
	CHECK(vector1.ToString() == "{ 2.000000, -5.000000 }");

	SW::Vector2<f32> vector2 = { 2.123456789f, -5.987654321f };
	CHECK(vector2.ToString() == "{ 2.123457, -5.987654 }");
}

TEST_CASE("Vector2 - T* ValuePtr - tests") 
{
	SW::Vector2<f32> vector = { 2.f, -5.f };

	CHECK(vector.ValuePtr() == &vector.x);
	CHECK(vector.ValuePtr() + 1 == &vector.y);
}

TEST_CASE("Vector2 - T Length - tests")
{
	SW::Vector2<f32> vector = { 2.f, -5.f };

	CHECK(vector.Length() == 5.3851648f);
}

TEST_CASE("Vector2 - void Normalize - tests")
{
	SW::Vector2<f32> vector = { 2.f, -5.f };

	vector.Normalize();

	CHECK(vector.x == 0.37139067f);
	CHECK(vector.y == -0.92847669f);

	CHECK(vector.Length() == 1.f);

	SW::Vector2<f32> vector2 = { 0.f, 0.f };

	vector2.Normalize();

	CHECK(vector2.x == 0.f);
	CHECK(vector2.y == 0.f);

	CHECK(vector2.Length() == 0.f);
}

TEST_CASE("Vector2 - void Normalized - tests")
{
	SW::Vector2<f32> vector = { 2.f, -5.f };

	SW::Vector2<f32> normalizedVector = vector.Normalized();

	CHECK(normalizedVector.x == 0.37139067f);
	CHECK(normalizedVector.y == -0.92847669f);
	CHECK(vector.x == 2.f);
	CHECK(vector.y == -5.f);

	CHECK(normalizedVector.Length() == 1.f);

	SW::Vector2<f32> vector2 = { 0.f, 0.f };

	SW::Vector2<f32> normalizedVector2 = vector2.Normalized();

	CHECK(normalizedVector2.x == 0.f);
	CHECK(normalizedVector2.y == 0.f);
	CHECK(vector2.x == 0.f);
	CHECK(vector2.y == 0.f);

	CHECK(normalizedVector2.Length() == 0.f);
}

TEST_CASE("Vector2 - T Dot - tests")
{
	SW::Vector2<f32> vector1 = { 2.f, -5.f };
	SW::Vector2<f32> vector2 = { 3.f, 4.f };

	CHECK(vector1.Dot(vector2) == -14.f);
	CHECK(vector2.Dot(vector1) == -14.f);

	CHECK(vector1.Dot(vector1) == 29.f);
	CHECK(vector2.Dot(vector2) == 25.f);

	SW::Vector2<f32> vector3 = { 0.f, 0.f };
	SW::Vector2<f32> vector4 = { 0.f, 0.f };

	CHECK(vector3.Dot(vector4) == 0.f);
	CHECK(vector4.Dot(vector3) == 0.f);

	CHECK(vector3.Dot(vector3) == 0.f);
	CHECK(vector4.Dot(vector4) == 0.f);
}

TEST_CASE("Vector2 - Vector2 to Vector2 operators - tests")
{
	SUBCASE("Vector2 += Vector2")
	{
		SW::Vector2<f32> vector1 = { 2.f, -5.f };
		SW::Vector2<f32> vector2 = { 3.f, 4.f };

		vector1 += vector2;

		Vector2ContentCheck(vector1, 5.f, -1.f);
	}

	SUBCASE("Vector2 -= Vector2")
	{
		SW::Vector2<f32> vector1 = { 2.f, -5.f };
		SW::Vector2<f32> vector2 = { 3.f, 4.f };

		vector1 -= vector2;

		Vector2ContentCheck(vector1, -1.f, -9.f);
	}

	SUBCASE("Vector2 *= Vector2")
	{
		SW::Vector2<f32> vector1 = { 2.f, -5.f };
		SW::Vector2<f32> vector2 = { 3.f, 4.f };

		vector1 *= vector2;

		Vector2ContentCheck(vector1, 6.f, -20.f);
	}

	SUBCASE("Vector2 /= Vector2")
	{
		SW::Vector2<f32> vector1 = { 2.f, -5.f };
		SW::Vector2<f32> vector2 = { 3.f, 4.f };

		vector1 /= vector2;

		Vector2ContentCheck(vector1, 0.66666669f, -1.25f);

		SW::Vector2<f32> vector3 = { 0.f, 0.f };
		SW::Vector2<f32> vector4 = { 3.f, 4.f };

		vector4 /= vector3;

		Vector2ContentCheck(vector4, 3.f, 4.f);
	}

	SUBCASE("Vector2 == Vector2")
	{
		SW::Vector2<f32> vector1 = { 2.f, -5.f };
		SW::Vector2<f32> vector2 = { 2.f, -5.f };

		CHECK(vector1 == vector2);
	}

	SUBCASE("Vector2 != Vector2")
	{
		SW::Vector2<f32> vector1 = { 2.f, -5.f };
		SW::Vector2<f32> vector2 = { 3.f, 4.f };

		CHECK(vector1 != vector2);
	}

	SUBCASE("Vector2 + Vector2")
	{
		SW::Vector2<f32> vector1 = { 2.f, -5.f };
		SW::Vector2<f32> vector2 = { 3.f, 4.f };

		SW::Vector2<f32> vector3 = vector1 + vector2;

		Vector2ContentCheck(vector3, 5.f, -1.f);
	}

	SUBCASE("Vector2 - Vector2")
	{
		SW::Vector2<f32> vector1 = { 2.f, -5.f };
		SW::Vector2<f32> vector2 = { 3.f, 4.f };

		SW::Vector2<f32> vector3 = vector1 - vector2;

		Vector2ContentCheck(vector3, -1.f, -9.f);
	}

	SUBCASE("Vector2 * Vector2")
	{
		SW::Vector2<f32> vector1 = { 2.f, -5.f };
		SW::Vector2<f32> vector2 = { 3.f, 4.f };

		SW::Vector2<f32> vector3 = vector1 * vector2;

		Vector2ContentCheck(vector3, 6.f, -20.f);
	}

	SUBCASE("Vector2 / Vector2")
	{
		SW::Vector2<f32> vector1 = { 2.f, -5.f };
		SW::Vector2<f32> vector2 = { 3.f, 4.f };

		SW::Vector2<f32> vector3 = vector1 / vector2;

		Vector2ContentCheck(vector3, 0.66666669f, -1.25f);

		SW::Vector2<f32> vector4 = { 0.f, 0.f };
		SW::Vector2<f32> vector5 = { 3.f, 4.f };

		SW::Vector2<f32> vector6 = vector5 / vector4;

		Vector2ContentCheck(vector6, 3.f, 4.f);
	}
}
