/**
 * @file Project.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-03-12
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

	/**
	 * @brief Represents a project's configuration.
	 */
	struct ProjectConfig
	{
		std::string Name = "Untitled"; /**< The name of the project. */
		std::filesystem::path AssetsDirectory = "assets"; /**< The directory where project assets are stored. */
	};
	
	/**
	 * @brief Represents a project in the software engine.
	 */
	class Project final
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		Project() = default;

		/**
		 * @brief Constructor that initializes the project with a given configuration.
		 * @param config The configuration for the project.
		 */
		Project(const ProjectConfig& config);

		/**
		 * @brief Default destructor.
		 */
		~Project() = default;

		/**
		 * @brief Retrieves the configuration of the project.
		 * @return The configuration of the project.
		 */
		const ProjectConfig& GetConfig() const { return m_Config; }

		/**
		 * @brief Retrieves the asset directory of the project.
		 * @return The asset directory of the project.
		 */
		const std::filesystem::path& GetAssetDirectory() const { return m_Config.AssetsDirectory; }

		/**
		 * @brief Retrieves the name of the project.
		 * @return The name of the project.
		 */
		const std::string& GetName() const { return m_Config.Name; }

	private:
		ProjectConfig m_Config;
	};

}
