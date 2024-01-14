/**
 * @file GuiLayer.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.0.1
 * @date 2024-01-03
 * 
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Core/Layer.hpp"

namespace SW {

	/**
	 * @brief GuiLayer is a layer that is responsible for rendering GUI.
	 * @warning Methods Begin() and End() must be called before and after rendering each frame of GUI.
	 */
	class GuiLayer final : public Layer
	{
	public:
		/**
		 * @brief Construct a new GuiLayer object.
		 */
		GuiLayer();

		/**
		 * @brief Destroy the GuiLayer object.
		 */
		~GuiLayer() override = default;

		/**
		 * @brief Called when layer is attached to the application.
		 */
		void OnAttach() override;

		/**
		 * @brief Called when layer is detached from the application.
		 */
		void OnDetach() override;

		/**
		 * @brief Called every frame to update the layer.
		 * 
		 * @param dt Time elapsed since the last frame.
		 */
		void OnUpdate(Timestep dt) override;

		/**
		 * @brief Called every frame to render the layer.
		 * @warning Not implemented in this layer.
		 */
		void OnRender() override;

		/**
		 * @brief Begin rendering GUI.
		 * @warning Must be called before rendering each frame of GUI.
		 */
		void Begin() const;
		
		/**
		 * @brief End rendering GUI.
		 * @warning Must be called after rendering each frame of GUI.
		 */
		void End() const;
	};
	
}
