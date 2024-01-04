/**
 * @file Math.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

    namespace Math {

		/**
		 * @brief Converts degrees to radians.
		 * 
		 * @param degrees Degrees to convert. 
		 * @return constexpr f32 
		 */
        constexpr f32 ToRadians(f32 degrees) {
            return degrees * 0.017453292519943295f;
        }

		/**
		 * @brief Converts radians to degrees.
		 * 
		 * @param radians Radians to convert.
		 * @return constexpr f32 
		 */
        constexpr f32 ToDegrees(f32 radians) {
            return radians * 57.29577951308232f;
        }

		/**
		 * @brief Approximation of PI constant.
		 */
        constexpr f32 PI = 3.14159265358979323846264338327950288f;

    }

}
