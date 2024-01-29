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

#include <imgui.h>
#include <imgui_internal.h>

namespace SW {

	/**
	 * @brief Get the total size of all allocations made by the engine.
	 * 
	 * @return size_t The total size of all allocations made by the engine.
	 */
	size_t GetAllocationSizeSum();

}
