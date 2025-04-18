/**
 * @file Layer.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.2
 * @date 2024-01-03
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Timestep.hpp"

namespace SW
{

	struct Event;

	/**
	 * @brief Abstract class representing a layer of the application.
	 * 		  Layers are used to separate different parts of the application.
	 * @note The user's specific code and functionalities should be inside the layers.
	 */
	class Layer
	{
	public:
		/**
		 * @brief Construct a new Layer object
		 *
		 * @param name Name of the layer
		 */
		Layer(const std::string& name = "DefaultLayer");

		/**
		 * @brief Destroy the Layer object
		 */
		virtual ~Layer() = default;

		/**
		 * @brief Called when the layer is attached to the application.
		 */
		virtual void OnAttach() = 0;

		/**
		 * @brief Called when the layer is detached from the application.
		 */
		virtual void OnDetach() = 0;

		/**
		 * @brief Called every frame to update the layer.
		 *
		 * @param dt Time elapsed since the last frame.
		 */
		virtual void OnUpdate(Timestep dt) = 0;

		/**
		 * @brief Called every frame to render the layer.
		 * @warning This function is called after OnUpdate().
		 * @note There user should but the user's ImGui rendering code.
		 */
		virtual void OnRender() = 0;

		/**
		 * @brief Get the Name of the layer
		 *
		 * @return const std::string& Name of the layer
		 */
		const std::string& GetName() const { return m_Name; }

	private:
		/**
		 * @brief Name of the layer
		 */
		std::string m_Name;
	};

} // namespace SW
