/**
 * @file OrthographicCamera.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.2
 * @date 2024-01-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Math/Matrix4.hpp"

namespace SW {

	/**
	 * @brief Orthographic camera class.
	 */
	class OrthographicCamera final
	{
	public:
		/**
		 * @brief Construct a new Orthographic Camera object.
		 *
		* @param left The left clipping plane.
		* @param right The right clipping plane.
		* @param bottom The bottom clipping plane.
		* @param top The top clipping plane.
		 */
		OrthographicCamera(f32 left, f32 right, f32 bottom, f32 top);

		/**
		 * @brief Get the Projection Matrix object
		 * @note The projection matrix is used to transform the camera's view space into a normalized space.
		 * 
		 * @return const Matrix4<f32>& 
		 */
		const Matrix4<f32>& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		/**
		 * @brief Get the View Matrix object
		 * @note The view matrix is used to transform from world space to camera space.
		 * 
		 * @return const Matrix4<f32>& 
		 */
		const Matrix4<f32>& GetViewMatrix() const { return m_ViewMatrix; }

		/**
		 * @brief Get the View Projection Matrix object
		 * @note The view projection matrix is the result of multiplying the view matrix by the projection 
		 * matrix. It is used to transform from world space directly to normalized device coordinates.
		 * 
		 * @return const Matrix4<f32>& 
		 */
		const Matrix4<f32>& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		/**
		 * @brief Get the Position object
		 * @note This returns the position of the camera in world space.
		 * 
		 * @return const Vector3<f32>& 
		 */
		const Vector3<f32>& GetPosition() const { return m_Position; }

		/**
		 * @brief Get the Rotation
		 * @note This returns the rotation of the camera around the Z-axis, in degrees.
		 * 
		 * @return f32 
		 */
		f32 GetRotation() const { return m_Rotation; }

		/**
		 * @brief Set the Projection parameters
		 * 
		 * This function sets the parameters for the orthographic projection matrix. 
		 * The parameters define the clipping planes of the view volume.
		 * 
		 * @param left The left clipping plane.
		 * @param right The right clipping plane.
		 * @param bottom The bottom clipping plane.
		 * @param top The top clipping plane.
		 */
		void SetProjection(f32 left, f32 right, f32 bottom, f32 top);

		/**
		 * @brief Set the Position object
		 * 
		 * This function sets the position of the camera in world space and recalculates the view matrix.
		 * 
		 * @param position The new position of the camera.
		 */
		void SetPosition(const Vector3<f32>& position) { m_Position = position; RecalculateViewMatrix(); }

		/**
		 * @brief Set the Rotation object
		 * 
		 * This function sets the rotation of the camera around the Z-axis, in degrees, and recalculates the view matrix.
		 * 
		 * @param rotation The new rotation of the camera.
		 */
		void SetRotation(f32 rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

	private:
		Matrix4<f32> m_ProjectionMatrix;	 /** @brief The projection matrix. */
		Matrix4<f32> m_ViewMatrix;			 /** @brief The view matrix. */
		Matrix4<f32> m_ViewProjectionMatrix; /** @brief The view projection matrix. */

		Vector3<f32> m_Position = { 0.0f };  /** @brief The camera position. */

		f32 m_Rotation = 0.0f;				 /** @brief The camera Z rotation. */

		/**
		 * @brief Recalculate the view matrix
		 * @warning It should be called whenever the position or rotation of the camera changes.
		 */
		void RecalculateViewMatrix();
	};

}
