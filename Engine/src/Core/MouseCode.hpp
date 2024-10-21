/**
 * @file MouseCode.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-02
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW
{

	/**
	 * @brief Enum for each possible mouse code.
	 */
	enum MouseCode : u16
	{
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast   = Button7,
		ButtonLeft   = Button0,
		ButtonRight  = Button1,
		ButtonMiddle = Button2
	};

} // namespace SW
