/**
 * @file Random.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.2
 * @date 2024-01-27
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <random>

namespace SW::Random {

    static std::mt19937 s_Generator(std::random_device{}());
    static std::uniform_int_distribution<u64> s_IDDistribution;

	/**
	 * @brief Creates a random ID.
	 *
	 * @return u64 The random ID.
	 */
	static u64 CreateID()
	{
		return s_IDDistribution(s_Generator);
	}

	/**
	 * @brief Creates a random tag.
	 * @param count The length of the tag.
	 * 
	 * @return std::string The random tag.
	 */
	static std::string CreateTag(int count = 10)
	{
		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		std::string tag;
		
		for (int i = 0; i < count; ++i)
			tag += alphanum[s_IDDistribution(s_Generator) % (sizeof(alphanum) - 1)];

		return tag;
	}

}
