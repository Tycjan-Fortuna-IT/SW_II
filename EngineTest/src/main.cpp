/*
	REQUIRE - this level will immediately quit the test case if the assert fails and will mark the test case as failed.
	CHECK - this level will mark the test case as failed if the assert fails but will continue with the test case.
	WARN - this level will only print a message if the assert fails but will not mark the test case as failed.
*/

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>

#include "Math_UT/Vector2_UT.hpp"

#include <Core/Debug/LogSystem.hpp>

int main(int argc, char** argv) {
	doctest::Context context;

	SW::LogSystem::Initialize();

	int res = context.run();

	SW::LogSystem::Shutdown();

	return res;
}
