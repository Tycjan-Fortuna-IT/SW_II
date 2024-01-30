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
#include "Core/Timestep.hpp"

namespace SW {

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
		 * @brief Destructor.
		 */
		~SceneCamera() override = default;

		/**
		 * @brief Sets the size of the viewport.
		 * @param width The width of the viewport.
		 * @param height The height of the viewport.
		 */
		void SetViewportSize(u32 width, u32 height);

		/**
		 * @brief Updates the camera based on the elapsed time since the last update.
		 * @param dt The elapsed time since the last update.
		 */
		void OnUpdate(Timestep dt);

		/**
		 * @brief Called when the viewport is resized.
		 * @param width The new width of the viewport.
		 * @param height The new height of the viewport.
		 */
		void OnViewportResize(f32 width, f32 height);

		/**
		 * @brief Called when the mouse is scrolled.
		 * @param xOffset The horizontal offset of the mouse scroll.
		 * @param yOffset The vertical offset of the mouse scroll.
		 */
		void OnMouseScrolled(f32 xOffset, f32 yOffset);

		/**
		 * @brief Gets the aspect ratio of the camera.
		 * @return The aspect ratio of the camera.
		 */
		f32 GetAspectRatio() const { return m_AspectRatio; }

		/**
		 * @brief Gets the orthographic size of the camera.
		 * @return The orthographic size of the camera.
		 */
		f32 GetOrthographicSize() const { return m_OrthographicSize; }

		/**
		 * @brief Gets the near clip distance of the orthographic projection.
		 * @return The near clip distance of the orthographic projection.
		 */
		f32 GetOrthographicNearClip() const { return m_OrthographicNear; }

		/**
		 * @brief Gets the far clip distance of the orthographic projection.
		 * @return The far clip distance of the orthographic projection.
		 */
		f32 GetOrthographicFarClip() const { return m_OrthographicFar; }

		/**
		 * @brief Sets the orthographic size of the camera.
		 * @param size The new orthographic size.
		 */
		void SetOrthographicSize(f32 size) { m_OrthographicSize = size; RecalculateProjection(); }

		/**
		 * @brief Sets the near clip distance of the orthographic projection.
		 * @param nearClip The new near clip distance.
		 */
		void SetOrthographicNearClip(f32 nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }

		/**
		 * @brief Sets the far clip distance of the orthographic projection.
		 * @param farClip The new far clip distance.
		 */
		void SetOrthographicFarClip(f32 farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

		/**
		 * @brief Gets the projection matrix of the camera.
		 * @return The projection matrix of the camera.
		 */
		const Matrix4<f32>& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		/**
		 * @brief Gets the view matrix of the camera.
		 * @return The view matrix of the camera.
		 */
		const Matrix4<f32>& GetViewMatrix() const { return m_ViewMatrix; }

		/**
		 * @brief Gets the view projection matrix of the camera.
		 * @return The view projection matrix of the camera.
		 */
		const Matrix4<f32>& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		f32 m_OrthographicSize = 10.0f;		/**< The orthographic size of the camera. */
		f32 m_OrthographicNear = -1.0f;		/**< The near clip distance of the orthographic projection. */
		f32 m_OrthographicFar = 1.0f;		/**< The far clip distance of the orthographic projection. */

		f32 m_AspectRatio = 0.0f;	/**< The aspect ratio of the camera. */
		f32 m_ZoomLevel = 1.0f;		/**< The zoom level of the camera. */

		Vector3<f32> m_CameraPosition = { 0.0f, 0.0f, 0.0f };	/**< The position of the camera. */
		f32 m_CameraRotation = 0.0f;							/**< The rotation of the camera. */

		f32 m_CameraMoveSpeed = 20.0f;		/**< The speed at which the camera moves. */
		f32 m_CameraRotationSpeed = 30.0f;	/**< The speed at which the camera rotates. */

		Matrix4<f32> m_ProjectionMatrix;		/**< The projection matrix of the camera. */
		Matrix4<f32> m_ViewMatrix;				/**< The view matrix of the camera. */
		Matrix4<f32> m_ViewProjectionMatrix;	/**< The view projection matrix of the camera. */

		/**
		 * @brief Recalculates the view matrix based on the camera's position and rotation.
		 */
		void RecalculateViewMatrix();

		/**
		 * @brief Recalculates the projection matrix based on the camera's orthographic settings.
		 */
		void RecalculateProjection();
	};

}
