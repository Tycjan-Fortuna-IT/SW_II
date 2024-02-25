#pragma once

#include <iostream>
#include <fstream>

#include <cstring>
#include <string>
#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

#include <filesystem>
#include <functional>
#include <utility>

#include "Core/Defines.hpp"
#include "Core/Debug/LogSystem.hpp"
#include "Core/Base.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SW {

	/**
	 * @brief Get the total size of all allocations made by the engine.
	 * 
	 * @return size_t The total size of all allocations made by the engine.
	 */
	size_t GetAllocationSizeSum();

}
