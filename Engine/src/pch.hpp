#pragma once

#include <fstream>
#include <iostream>

#include <array>
#include <cstring>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <filesystem>
#include <functional>
#include <utility>

#include <Logger/Logger.hpp>

#include "Core/Base.hpp"
#include "Core/Debug/Profiler.hpp"
#include "Core/Defines.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>

BEGIN_CAST_FORMATTER(std::filesystem::path);
FORMATTER_CAST(std::string, value.string());
END_CAST_FORMATTER;

BEGIN_ADV_FORMATTER(glm::vec2);
*it++ = '(';
it    = FORMATTER_FORMAT(float, value.x);
*it++ = ',';
*it++ = ' ';
it    = FORMATTER_FORMAT(float, value.y);
*it++ = ')';
END_ADV_FORMATTER;

BEGIN_ADV_FORMATTER(glm::vec3);
*it++ = '(';
it    = FORMATTER_FORMAT(float, value.x);
*it++ = ',';
*it++ = ' ';
it    = FORMATTER_FORMAT(float, value.y);
*it++ = ',';
*it++ = ' ';
it    = FORMATTER_FORMAT(float, value.z);
*it++ = ')';
END_ADV_FORMATTER;

BEGIN_ADV_FORMATTER(glm::vec4);
*it++ = '(';
it    = FORMATTER_FORMAT(float, value.x);
*it++ = ',';
*it++ = ' ';
it    = FORMATTER_FORMAT(float, value.y);
*it++ = ',';
*it++ = ' ';
it    = FORMATTER_FORMAT(float, value.z);
*it++ = ',';
*it++ = ' ';
it    = FORMATTER_FORMAT(float, value.w);
*it++ = ')';
END_ADV_FORMATTER;

namespace SW
{

	/**
	 * @brief Get the total size of all allocations made by the engine.
	 *
	 * @return size_t The total size of all allocations made by the engine.
	 */
	size_t GetAllocationSizeSum();

} // namespace SW
