/**
 * @file PropertiesPanel.hpp
 * @version 0.1.0
 * @date 2024-05-12
 *
 * @copyright Copyright (c) SW
 */
#pragma once

#include "GUI/Panel.hpp"
#include "SceneViewportPanel.hpp"

namespace SW {

	/**
	 * @brief The PropertiesPanel class represents a panel that displays for the selected object its properties and components.
	 * 		  Allows to edit the properties and add/remove components.
	 */
	class PropertiesPanel final : public Panel
	{
	public:
		/**
		 * @brief Constructs a PropertiesPanel object with an optional name.
		 * @param context Current scene context.
		 */
		PropertiesPanel(SceneViewportPanel* sceneViewportPanel);
		~PropertiesPanel() override;

		PropertiesPanel(const PropertiesPanel& other) = delete;
		PropertiesPanel(PropertiesPanel&& other) = delete;
		PropertiesPanel& operator=(const PropertiesPanel& other) = delete;
		PropertiesPanel& operator=(PropertiesPanel&& other) = delete;

		PanelType GetPanelType() const override { return PanelType::PropertiesPanel; }

		/**
		 * @brief Called every frame to update the panel.
		 * @param dt The time since the last frame.
		 */
		void OnUpdate(UNUSED Timestep dt) override {}

		/**
		 * @brief Called every frame to render the panel.
		 */
		void OnRender() override;

		/**
		 * @brief Adds a component name to the PropertiesPanel.
		 * 
		 * This function is used to associate a component type with its corresponding name in the PropertiesPanel.
		 * 
		 * @tparam Component The type of the component.
		 * @param name The name of the component.
		 */
		template<typename Component>
		inline void AddComponentName(const std::string& name)
		{
			const size_t id = entt::type_id<Component>().hash();
			m_ComponentNames[id] = name;
		}

		/**
		 * @brief Template function to draw a component in the properties panel.
		 * 
		 * @tparam T The type of the component.
		 * @tparam Fn The type of the function to invoke on the component.
		 * @param entity The entity that contains the component.
		 * @param fn The function to invoke on the component.
		 * @param removable Flag indicating whether the component is removable.
		 */
		template<typename T, typename Fn>
			requires std::is_invocable_v<Fn, T&>
		void DrawComponent(Entity entity, Fn fn, bool removable = true)
		{
			if (!entity.HasComponent<T>())
				return;

			static constexpr ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen
				| ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_AllowItemOverlap
				| ImGuiTreeNodeFlags_Framed;

			T& component = entity.GetComponent<T>();

			const f32 lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + lineHeight * 0.25f);

			const size_t id = entt::type_id<T>().hash();
			const bool open = ImGui::TreeNodeEx(reinterpret_cast<void*>(id), treeFlags, "%s", m_ComponentNames[id].c_str());

			bool removeComponent = false;

			ImGui::PushID(static_cast<int>(id));

			const f32 frameHeight = ImGui::GetFrameHeight();

			ImGui::SameLine(ImGui::GetContentRegionMax().x - frameHeight * 1.f);

			if (ImGui::Button(SW_ICON_SETTINGS, ImVec2{ frameHeight * 1.2f, frameHeight }))
				ImGui::OpenPopup("ComponentSettings");

			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItemEx("Copy", SW_ICON_CONTENT_COPY)) {
					m_SceneViewportPanel->GetCurrentScene()->CopyComponentIntoScene<T>(m_ComponentCopyScene, entity, m_ComponentCopyEntity);
				}

				if (ImGui::MenuItemEx("Paste", SW_ICON_CONTENT_PASTE)) {
					m_ComponentCopyScene->CopyComponentIntoScene<T>(m_SceneViewportPanel->GetCurrentScene(), m_ComponentCopyEntity, entity);
				}

				if (removable) {
					if (ImGui::MenuItemEx("Remove", SW_ICON_DELETE)) {
						removeComponent = true;
					}
				}

				ImGui::EndPopup();
			}

			ImGui::PopID();

			if (open) {
				fn(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}

	private:
		SceneViewportPanel* m_SceneViewportPanel = nullptr;	/**< The current scene viewport context. */

		Entity m_ComponentCopyEntity;				/**< The entity that contains the component to copy. */
		Scene* m_ComponentCopyScene = nullptr;		/**< The scene that contains the component to copy. */

		std::unordered_map<size_t, std::string> m_ComponentNames;		/**< The map of component names. */

		void DrawAddComponentMenu(Entity entity);
	};

}
