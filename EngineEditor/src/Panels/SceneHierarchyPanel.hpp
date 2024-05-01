/**
 * @file SceneHierarchyPanel.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.3
 * @date 2024-03-04
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "GUI/Panel.hpp"
#include "Core/ECS/Components.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Scene/Scene.hpp"

namespace SW {

	class SceneViewportPanel;

	/**
	 * @brief The SceneHierarchyPanel class represents a panel that displays all entities in the scene
	 *        in a tree-like structure.
	 */
	class SceneHierarchyPanel final : public Panel
	{
	public:
		/**
		 * @brief Constructs a SceneHierarchyPanel object with an optional name.
		 * 
		 * @param context Current scene context.
		 */
		explicit SceneHierarchyPanel(SceneViewportPanel* sceneViewportPanel);

		/**
		 * @brief Default destructor for the SceneHierarchyPanel class.
		 */
		~SceneHierarchyPanel() override = default;

		/** @brief Copy constructor (deleted). */
		SceneHierarchyPanel(const SceneHierarchyPanel& other) = delete;
		/** @brief Move constructor (deleted). */
		SceneHierarchyPanel(SceneHierarchyPanel&& other) = delete;
		/** @brief Copy assignment operator (deleted). */
		SceneHierarchyPanel& operator=(const SceneHierarchyPanel& other) = delete;
		/** @brief Move assignment operator (deleted). */
		SceneHierarchyPanel& operator=(SceneHierarchyPanel&& other) = delete;

		/**
		 * @brief Called every frame to update the panel.
		 * @param dt The time since the last frame.
		 */
		void OnUpdate(Timestep dt) final override;

		/**
		 * @brief Called every frame to render the panel.
		 */
		void OnRender() final override;

		/**
		 * @brief Draws the entity node in the scene hierarchy panel. Recursively calls itself for each child entity.
		 * 
		 * @param entity The entity to draw.
		 * @param id The unique id of the entity.
		 * @param tc The tag component of the entity.
		 * @param rsc The relationship component of the entity.
		 * @param depth The depth of the entity in the hierarchy.
		 * @return ImRect The rectangle of the created entity node.
		 */
		ImRect RenderEntityNode(Entity entity, u64 id, TagComponent& tc, const RelationshipComponent& rsc, u32 depth = 0);

	private:
		Entity m_DraggedEntity;			/**< The entity that is currently being dragged. */
		Entity m_DraggedEntityTarget;	/**< The entity that the dragged entity is being dragged onto. */
		Entity m_EntityToDelete;		/**< The entity that is currently being deleted. */
		Entity m_RenamingEntity;		/**< The entity that is currently being renamed. */

		std::string m_SearchString;		/**< The search string used to filter entities in the scene hierarchy. */

		SceneViewportPanel* m_SceneViewportPanel = nullptr;	/**< The current scene viewport context. */

		/**
		 * @brief Draws the entity creation menu in the scene hierarchy panel.
		 *
		 * @param scene A pointer to the current scene.
		 * @param toParent (Optional) The entity to parent the newly created entity to.
		 */
		void DrawEntityCreateMenu(Scene* scene, Entity toParent = {});

		bool OnKeyPressed(KeyCode code);
	};

}
