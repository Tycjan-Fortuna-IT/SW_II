/*
	REQUIRE - this level will immediately quit the test case if the assert fails and will mark the test case as failed.
	CHECK - this level will mark the test case as failed if the assert fails but will continue with the test case.
	WARN - this level will only print a message if the assert fails but will not mark the test case as failed.
*/

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>

#define GLM_ENABLE_EXPERIMENTAL	// TODO: remove this
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Core/Defines.hpp>
#include <Core/Debug/LogSystem.hpp>

#include "Math_UT/Vector2_UT.hpp"
#include "Math_UT/Vector3_UT.hpp"
#include "Math_UT/Vector4_UT.hpp"

int main(int argc, char** argv) {
	doctest::Context context;

	SW::LogSystem::Initialize();

	int res = context.run();

	SW::LogSystem::Shutdown();

	return res;
}
