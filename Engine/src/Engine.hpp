#pragma once

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

#include <imgui.h>
#include <imgui_internal.h>

#include <string>
#include <memory>
#include <vector>
#include <filesystem>

#include "Core/Defines.hpp"
#include "Core/Debug/LogSystem.hpp"
#include "Core/Base.hpp"

#include "Core/Application.hpp"
#include "Core/Layer.hpp"
#include "Core/Timestep.hpp"
#include "Core/KeyCode.hpp"

#include "Core/Renderer/Renderer2D.hpp"

#include "Core/OpenGL/Shader.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Core/OpenGL/Framebuffer.hpp"

#include "Core/Utils/Random.hpp"
#include "Core/Utils/Input.hpp"
#include "Core/Utils/Utils.hpp"

#include "Core/Math/Math.hpp"
#include "Core/Math/Matrix4.hpp"
#include "Core/Math/Vector2.hpp"

#include "Core/Scene/Scene.hpp"
#include "Core/Scene/SceneCamera.hpp"

#include "Core/ECS/Components.hpp"

#include "Core/Events/Event.hpp"

#include "GUI/Appearance.hpp"
#include "GUI/Colors.hpp"
#include "GUI/GUI.hpp"
