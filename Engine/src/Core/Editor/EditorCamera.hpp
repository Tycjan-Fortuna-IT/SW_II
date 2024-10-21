/**
 * @file Scene.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-02-06
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Renderer/Camera.hpp"
#include "Core/Timestep.hpp"

namespace SW
{

	/**
	 * @brief The EditorCamera class represents a camera used in the editor.
	 *
	 * This class inherits from the Camera class and provides additional functionality specific to the editor.
	 */
	class EditorCamera final : public Camera
	{
	public:
		/**
		 * @brief Default constructor for EditorCamera.
		 */
		EditorCamera() = default;

		/**
		 * @brief Constructor for EditorCamera.
		 *
		 * @param fov The field of view angle in degrees.
		 * @param aspectRatio The aspect ratio of the camera.
		 * @param nearClip The near clipping plane distance.
		 * @param farClip The far clipping plane distance.
		 */
		EditorCamera(f32 fov, f32 aspectRatio, f32 nearClip, f32 farClip);

		/**
		 * @brief Updates the camera based on the given timestep.
		 *
		 * @param timestep The timestep value.
		 */
		void OnUpdate(UNUSED Timestep timestep);

		/**
		 * @brief Sets the viewport size of the camera.
		 *
		 * @param width The width of the viewport.
		 * @param height The height of the viewport.
		 */
		void SetViewportSize(f32 width, f32 height);

		/**
		 * @brief Sets the position of the camera.
		 *
		 * @param position The new position of the camera.
		 */
		void SetPosition(const glm::vec3& position) { m_Position = position; }

		/**
		 * @brief Sets the yaw angle of the camera.
		 *
		 * @param yaw The new yaw angle in radians.
		 */
		void SetYaw(f32 yaw) { m_Yaw = yaw; }

		/**
		 * @brief Sets the pitch angle of the camera.
		 *
		 * @param pitch The new pitch angle in radians.
		 */
		void SetPitch(f32 pitch) { m_Pitch = pitch; }

		/**
		 * @brief Gets the view matrix of the camera.
		 *
		 * @return The view matrix.
		 */
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		/**
		 * @brief Gets the view projection matrix of the camera.
		 *
		 * @return The view projection matrix.
		 */
		glm::mat4 GetViewProjectionMatrix() const { return m_Projection * m_ViewMatrix; }

		/**
		 * @brief Gets the position of the camera.
		 *
		 * @return The position of the camera.
		 */
		const glm::vec3& GetPosition() const { return m_Position; }

		/**
		 * @brief Gets the forward vector of the camera.
		 *
		 * @return The forward vector.
		 */
		const glm::vec3& GetForward() const { return m_Forward; }

		/**
		 * @brief Gets the right vector of the camera.
		 *
		 * @return The right vector.
		 */
		const glm::vec3& GetRight() const { return m_Right; }

		/**
		 * @brief Gets the up vector of the camera.
		 *
		 * @return The up vector.
		 */
		const glm::vec3& GetUp() const { return m_Up; }

		/**
		 * @brief Gets the yaw angle of the camera.
		 *
		 * @return The yaw angle in radians.
		 */
		f32 GetYaw() const { return m_Yaw; }

		/**
		 * @brief Gets the pitch angle of the camera.
		 *
		 * @return The pitch angle in radians.
		 */
		f32 GetPitch() const { return m_Pitch; }

		/**
		 * @brief Gets the aspect ratio of the camera.
		 * @return The aspect ratio of the camera.
		 */
		f32 GetAspectRatio() const { return m_AspectRatio; }

	private:
		f32 m_Fov         = glm::radians(45.0f); /** @brief The field of view angle in radians. */
		f32 m_AspectRatio = 1.777f;              /** @brief The aspect ratio of the camera. */
		f32 m_NearClip    = 0.03f;               /** @brief The near clipping plane distance. */
		f32 m_FarClip     = 2000.0f;             /** @brief The far clipping plane distance. */

		glm::vec3 m_Position = glm::vec3(0, 0, 10); /** @brief The position of the camera. */
		glm::vec3 m_Forward  = glm::vec3(0, 0, 1);  /** @brief The forward vector of the camera. */
		glm::vec3 m_Right    = glm::vec3(1, 0, 0);  /** @brief The right vector of the camera. */
		glm::vec3 m_Up       = glm::vec3(0, 1, 0);  /** @brief The up vector of the camera. */

		f32 m_Yaw   = glm::radians(270.0f); /** @brief The yaw angle in radians. */
		f32 m_Pitch = glm::radians(0.0f);   /** @brief The pitch angle in radians. */

		glm::mat4 m_ViewMatrix = glm::mat4(1.0f); /** @brief The view matrix of the camera. */
	};

} // namespace SW
