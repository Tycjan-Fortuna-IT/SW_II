/**
 * @file Camera.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-02-04
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW
{

	/**
	 * @brief Represents a camera in the rendering system.
	 */
	class Camera
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		Camera() = default;

		Camera(const glm::mat4& projection) : m_Projection(projection) {}

		/**
		 * @brief Get the projection matrix of the camera.
		 * @return The projection matrix.
		 */
		const glm::mat4& GetProjectionMatrix() const { return m_Projection; }

	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f); /** @brief The projection matrix of the camera. */
	};

} // namespace SW
