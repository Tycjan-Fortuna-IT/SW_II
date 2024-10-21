/**
 * @file InternalCallManager.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-08
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace Coral
{
	class ManagedAssembly;
}

namespace SW
{

	class InternalCallManager
	{
	public:
		static void Initialize(Coral::ManagedAssembly* coreAssembly);

	private:
		static void RegisterTypes(Coral::ManagedAssembly* coreAssembly);
		static void RegisterInternalCalls(Coral::ManagedAssembly* coreAssembly);
	};

} // namespace SW
