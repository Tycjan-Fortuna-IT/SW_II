/**
 * @file SceneCamera.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.2
 * @date 2024-01-20
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Renderer/Camera.hpp"

namespace SW {

	enum class ProjectionType : u8
	{
		Perspective = 0,
		Orthographic = 1
	};

	/**
	 * @class SceneCamera
	 * @brief Represents a camera used in a scene.
	 * 
	 * The SceneCamera class inherits from the Camera class and provides additional functionality
	 * specific to scene cameras, such as viewport resizing, mouse scrolling, and camera movement.
	 */
	class SceneCamera final : public Camera
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		SceneCamera() = default;

		/**
		 * @brief Constructor that takes the aspect ratio of the camera.
		 * @param aspectRatio The aspect ratio of the camera.
		 */
		SceneCamera(f32 aspectRatio);

		/**
		 * @brief Sets the size of the viewport.
		 * @param width The width of the viewport.
		 * @param height The height of the viewport.
		 */
		void SetViewportSize(u32 width, u32 height);

		/**
		 * @brief Gets the aspect ratio of the camera.
		 * @return The aspect ratio of the camera.
		 */
		f32 GetAspectRatio() const { return m_AspectRatio; }

		void SetPerspective(f32 verticalFov, f32 nearClip, f32 farClip);
		void SetOrthographic(f32 size, f32 nearClip, f32 farClip);

		f32 GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(f32 verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }

		f32 GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(f32 nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }

		f32 GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(f32 farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

		f32 GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(f32 size) { m_OrthographicSize = size; RecalculateProjection(); }

		f32 GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(f32 nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }

		f32 GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(f32 farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

	private:
		f32 m_OrthographicSize = 10.0f;		/**< The orthographic size of the camera. */
		f32 m_OrthographicNear = -1.0f;		/**< The near clip distance of the orthographic projection. */
		f32 m_OrthographicFar = 1.0f;		/**< The far clip distance of the orthographic projection. */

		f32 m_PerspectiveFOV = glm::radians(45.0f);
		f32 m_PerspectiveNear = 0.01f;
		f32 m_PerspectiveFar = 1000.0f;

		f32 m_AspectRatio = 0.0f;	/**< The aspect ratio of the camera. */

		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		/**
		 * @brief Recalculates the projection matrix based on the camera's orthographic settings.
		 */
		void RecalculateProjection();
	};

}
