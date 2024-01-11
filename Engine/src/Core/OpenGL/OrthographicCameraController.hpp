/**
 * @file OrthographicCameraController.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "OrthographicCamera.hpp"

namespace SW {

	/**
	 * @brief Orthographic camera controller.
	 * 		  Allows for moving and rotating the camera.
	 */
	class OrthographicCameraController final
	{
	public:
		/**
		 * @brief Construct a new Orthographic Camera Controller
		 * 
		 * @param aspectRatio The aspect ratio of the camera. e.g. 16:9
		 */
		OrthographicCameraController(f32 aspectRatio);

		/**
		 * @brief Orthographic camera controller destructor.
		 */
		~OrthographicCameraController() = default;

		/**
		 * @brief Orthographic camera controller update function.
		 * 
		 * @param dt Delta time.
		 */
		void OnUpdate(f32 dt);

		/**
		 * @brief Orthographic camera controller method to resize the camera view.
		 * 
		 * @param width New width.
		 * @param height New height.
		 */
		void OnResize(f32 width, f32 height);

		/**
		 * @brief Get the Camera
		 * 
		 * @return const OrthographicCamera& 
		 */
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		/**
		 * @brief Set the Zoom Level
		 * 
		 * @param level New zoom level.
		 */
		void SetZoomLevel(f32 level) { m_ZoomLevel = level; CalculateView(); }

		/**
		 * @brief React on the mouse scrolled event.
		 *
		 * @param xOffset Offset on the X axis.
		 * @param yOffset Offset on the Y axis.
		 * @return bool
		 */
		bool OnMouseScrolled(f32 xOffset, f32 yOffset);

		/**
		 * @brief React on the window resized event.
		 *
		 * @param width New width.
		 * @param height New height.
		 * @return bool
		 */
		bool OnWindowResized(i32 width, i32 height);

	private:
		f32 m_AspectRatio;      	/** @brief Aspect ratio of the camera. */
		f32 m_ZoomLevel = 1.0f;		/** @brief Zoom level of the camera. */

		Vector3<f32> m_CameraPosition = { 0.0f, 0.0f, 0.0f }; /** @brief Position of the camera. */

		f32 m_CameraRotation = 0.0f; 		/** @brief Rotation of the camera. */
		f32 m_CameraMoveSpeed = 20.0f; 		/** @brief Speed of the camera. */
		f32 m_CameraRotationSpeed = 20.0f; 	/** @brief Rotation speed of the camera. */

		OrthographicCamera m_Camera; /** @brief Orthographic camera. */

		/**
		 * @brief Calculate or recalculate the view of the camera.
		 */
		void CalculateView();
	};

}
