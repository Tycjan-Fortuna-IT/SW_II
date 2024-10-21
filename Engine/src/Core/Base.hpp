/**
 * @file Base.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-29
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW
{

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

} // namespace SW
