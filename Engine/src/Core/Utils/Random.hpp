#pragma once

#include <random>

namespace SW {

    static std::mt19937 s_Generator;
    static std::uniform_int_distribution<u64> s_IDDistribution;

    static u64 CreateID()
	{
        return s_IDDistribution(s_Generator);
    }

}
