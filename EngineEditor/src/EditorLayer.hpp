#pragma once

#include <Engine.hpp>

#include <imgui_internal.h>

#include "Panels/SceneViewportPanel.hpp"

namespace SW {

	class Panel;

	class EditorLayer final : public Layer {
	public:
		explicit EditorLayer(const std::string& name)
			: Layer(name) {}

		std::vector<Panel*> m_Panels;

		std::shared_ptr<Shader> m_Shader = nullptr;

		Texture2D* m_IconTexture = nullptr;
		Texture2D* m_CloseIconTexture = nullptr;
		Texture2D* m_MaximizeIconTexture = nullptr;
		Texture2D* m_MinimizeIconTexture = nullptr;
		Texture2D* m_RestoreIconTexture = nullptr;

		SceneViewportPanel* m_Viewport = nullptr;

		bool m_WindowMaximized = true;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep dt) override;
		void OnRender() override;

		bool OnKeyPressed(KeyCode code);

		f32 DrawTitleBar();
		void DrawMenuBar();

		void OpenProject();
		void SaveProjectAs();
	};

}
