/**
 * @file Project.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-03-12
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset/AssetManagerBase.hpp"
#include "Asset/EditorAssetManager.hpp"

namespace SW {

	/**
	 * @brief Represents a project's configuration.
	 */
	struct ProjectConfig
	{
		std::string Name = "Untitled";											/**< The name of the project. */
		std::filesystem::path AssetsDirectory = "assets";						/**< The directory where project assets are stored. */
		std::filesystem::path AssetRegistryPath = "assets/assets.sw_registry";	/**< The path to the asset registry file. */
		std::filesystem::path AudioRegistryPath = "assets/audio.sw_registry";	/**< The path to the audio registry file. */
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
		~Project();

		void Initialize();

		/**
		 * @brief Retrieves the configuration of the project.
		 * @return The configuration of the project.
		 */
		const ProjectConfig& GetConfig() const { return m_Config; }

		/**
		 * @brief Retrieves the asset directory of the project.
		 * @return The asset directory of the project. e.g. C:\Users\x\Desktop\Testbed\assets
		 */
		const std::filesystem::path& GetAssetDirectory() const { return m_Config.AssetsDirectory; }

		/**
		 * @brief Retrieves the asset parent directory of the project.
		 * @return The asset directory of the project. e.g. C:\Users\x\Desktop\Testbed
		 */
		const std::filesystem::path& GetAssetParentDirectory() const { return m_Config.AssetsDirectory.parent_path(); }

		/**
		 * @brief Retrieves the asset registry path of the project.
		 * @return The asset registry path of the project. e.g. C:\Users\x\Desktop\Testbed\assets\assets.sw_registry
		 */
		const std::filesystem::path& GetAssetRegistryPath() const { return m_Config.AssetRegistryPath; }

		/**
		 * @brief Retrieves the audio registry path of the project.
		 * @return The audio registry path of the project. e.g. C:\Users\x\Desktop\Testbed\assets\audio.sw_registry
		 */
		const std::filesystem::path& GetAudioRegistryPath() const { return m_Config.AudioRegistryPath; }
		
		/**
		 * @brief Retrieves the name of the project.
		 * @return The name of the project.
		 */
		const std::string& GetName() const { return m_Config.Name; }

		ProjectConfig& GetConfig() { return m_Config; }

		EditorAssetManager* GetEditorAssetManager() const { return dynamic_cast<EditorAssetManager*>(m_AssetManager); }
		AssetManagerBase* GetAssetManager() const { return m_AssetManager; }

	private:
		ProjectConfig m_Config;
		AssetManagerBase* m_AssetManager = nullptr;
	};

}
