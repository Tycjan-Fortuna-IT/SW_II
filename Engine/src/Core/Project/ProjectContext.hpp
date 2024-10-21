/**
 * @file ProjectContext.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-02-20
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Events/Event.hpp"
#include "Project.hpp"

namespace SW
{

	/**
	 * @brief The ProjectContext class represents the context of the current project.
	 */
	class ProjectContext final
	{
	public:
		/**
		 * @brief Checks if a project context exists.
		 * @return true if a project context exists, false otherwise.
		 */
		static bool HasContext() { return s_CurrentProject != nullptr; }

		/**
		 * @brief Sets the current project.
		 * @param project A pointer to the project to set as the current project.
		 */
		static void Set(Project* project)
		{
			s_CurrentProject = project;
			s_CurrentProject->Initialize();

			EventSystem::Emit(EVENT_CODE_PROJECT_LOADED);
		}

		/**
		 * @brief Gets the current project.
		 * @return A pointer to the current project.
		 */
		static Project* Get() { return s_CurrentProject; }

	private:
		static Project* s_CurrentProject; ///< Pointer to the current project.
	};

} // namespace SW
