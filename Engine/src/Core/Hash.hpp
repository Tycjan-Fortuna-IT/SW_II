/**
 * @file Hash.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-09
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW
{

	class Hash
	{
	public:
		static constexpr u32 GenerateFNVHash(std::string_view str)
		{
			constexpr u32 FNV_PRIME    = 16777619u;
			constexpr u32 OFFSET_BASIS = 2166136261u;

			const u64 length = str.length();
			const char* data = str.data();

			u32 hash = OFFSET_BASIS;

			for (u64 i = 0; i < length; ++i)
			{
				hash ^= *data++;
				hash *= FNV_PRIME;
			}

			hash ^= '\0';
			hash *= FNV_PRIME;

			return hash;
		}
	};

} // namespace SW
