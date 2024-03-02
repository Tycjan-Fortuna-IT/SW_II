#pragma once

namespace SW {

	class Panel;
	class SceneViewportPanel;

	class EditorLayer final : public Layer {
	public:
		explicit EditorLayer(const std::string& name)
			: Layer(name) {}

		std::vector<Panel*> m_Panels;

		Texture2D* m_IconTexture = nullptr;
		Texture2D* m_CloseIconTexture = nullptr;
		Texture2D* m_MaximizeIconTexture = nullptr;
		Texture2D* m_MinimizeIconTexture = nullptr;
		Texture2D* m_RestoreIconTexture = nullptr;

		SceneViewportPanel* m_Viewport = nullptr;

		bool m_WindowMaximized = true;

		bool m_OpenNewSceneModal = false;
		bool m_IsNewSceneModalOpen = false;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep dt) override;
		void OnRender() override;

		bool OnKeyPressed(KeyCode code);

		f32 DrawTitleBar();
		void DrawMenuBar();

		void CreateNewScene();
		void SaveCurrentScene();

		void OpenProject();
		void SaveProjectAs();
	};

}
