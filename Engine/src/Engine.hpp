#pragma once

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

#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <Eventing/Eventing.hpp>
#include <Logger/Logger.hpp>

#include <OpenGL/Input/KeyCode.hpp>
#include <OpenGL/Input/MouseCode.hpp>

#include "Core/Base.hpp"
#include "Core/Debug/Profiler.hpp"
#include "Core/Defines.hpp"

#include "Core/Application.hpp"
#include "Core/Layer.hpp"
#include "Core/Timestep.hpp"

#include "Core/Renderer/Renderer2D.hpp"

#include "Core/Utils/Random.hpp"
#include "Core/Utils/Utils.hpp"

#include "Core/Math/Math.hpp"
// #include "Core/Math/Matrix4.hpp"
// #include "Core/Math/Vector2.hpp"

#include "Core/Scene/Scene.hpp"
#include "Core/Scene/SceneCamera.hpp"

#include "Core/ECS/Components.hpp"

#include "GUI/Appearance.hpp"
#include "GUI/Colors.hpp"
#include "GUI/GUI.hpp"
