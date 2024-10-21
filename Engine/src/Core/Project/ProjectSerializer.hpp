/**
 * @file ProjectSerializer.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-02-20
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW
{

	class Project;

	class ProjectSerializer final
	{
	public:
		/**
		 * @brief Serializes project to file.
		 *
		 * @param project Project to serialize.
		 * @param path Path to file.
		 */
		static void Serialize(Project* project, const std::string& path);

		/**
		 * @brief Deserializes project from file.
		 *
		 * @param project Project to deserialize.
		 * @param path Path to file.
		 */
		[[nodiscard]] static Project* Deserialize(const std::string& path);
	};

} // namespace SW
