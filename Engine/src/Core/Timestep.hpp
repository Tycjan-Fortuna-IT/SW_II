/**
 * @file Timestep.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-14
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW
{

	/**
	 * @brief Represents a time interval in seconds.
	 *
	 * The Timestep class is used to represent a time interval in seconds.
	 * It provides methods to retrieve the time in seconds and milliseconds.
	 */
	class Timestep final
	{
	public:
		/**
		 * @brief Constructs a Timestep object with the specified time.
		 *
		 * @param time The time interval in seconds.
		 */
		Timestep(f32 time = 0.0f) : m_Time(time) {}

		/**
		 * @brief Converts the Timestep object to a float value representing the time in seconds.
		 *
		 * @return The time interval in seconds.
		 */
		operator f32() const { return m_Time; }

		/**
		 * @brief Gets the time interval in seconds.
		 *
		 * @return The time interval in seconds.
		 */
		[[nodiscard]] f32 GetSeconds() const { return m_Time; }

		/**
		 * @brief Gets the time interval in milliseconds.
		 *
		 * @return The time interval in milliseconds.
		 */
		[[nodiscard]] f32 GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
		f32 m_Time; ///< The time interval in seconds.
	};

} // namespace SW
