/**
 * @file AssetEditorPanel.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset/Asset.hpp"
#include "Core/Timestep.hpp"
#include "Core/Scene/Scene.hpp"

namespace SW {

	/**
	 * @class AssetEditorPanel
	 * @brief Base class for asset editor panels.
	 *
	 * The AssetEditorPanel class provides a base for creating custom panels in an asset editor.
	 * It defines common functionality and interface that derived panel classes can implement.
	 */
	class AssetEditorPanel
	{		
	protected:
		AssetEditorPanel(const char* name, const char* icon);

	public:
		virtual ~AssetEditorPanel() = default;

		/**
		 * @brief Updates the panel.
		 * @param ts The timestep.
		 */
		virtual void OnUpdate(Timestep ts) = 0;
		
		/**
		 * @brief Renders all the panels.
		 */
		virtual void OnRender();

		/**
		 * @brief Opens the panel.
		 */
		void Open();

		/**
		 * @brief Closes the panel.
		 */
		void Close();

		/**
		 * @brief Returns whether the panel is open.
		 * 
		 * @return True if the panel is open, false otherwise.
		 */
		bool IsOpen() const { return m_IsOpen; }

		/**
		 * @brief Sets the scene context for the panel.
		 * @param scene The scene context.
		 */
		void SetSceneContext(Scene* scene) { m_Scene = scene; }

		/**
		 * @brief Set the asset handle for the panel.
		 * @note This determines the asset that the panel will edit.

		 * @param handle The asset handle. 
		 */
		virtual void SetAssetHandle(AssetHandle handle) = 0;
	
	protected:
		/**
		 * @brief Pushes the additional window style for the panel.
		 */
		virtual void OnWindowStylePush() {}

		/**
		 * @brief Pops the additional window style for the panel.
		 */
		virtual void OnWindowStylePop() {}

		std::string m_ID;   ///< The ID of the panel.
		std::string m_Name; ///< The name of the panel.
		std::string m_Icon; ///< The icon of the panel.

		ImVec2 m_MinSize, m_MaxSize;

		bool m_IsOpen = false;

		Scene* m_Scene = nullptr;

	private:
		/**
		 * @brief After the panel is opened, this function is called.
		 */
		virtual void OnOpen() = 0;
		
		/**
		 * @brief After the panel is closed, this function is called.
		 */
		virtual void OnClose() = 0;
		
		/**
		 * @brief Renders the panel.
		 */
		virtual void Render() = 0;
	};

}

