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

	class GuiLayer final : public Layer
	{
	public:
		GuiLayer();
		~GuiLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float dt) override;
		void OnRender() override;
		void Begin() const;
		void End() const;
	};
	
}
