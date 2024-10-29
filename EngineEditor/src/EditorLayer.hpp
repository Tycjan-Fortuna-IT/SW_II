#pragma once

namespace SW
{
	class SceneViewportPanel;

	class EditorLayer final : public Layer
	{
	public:
		explicit EditorLayer(const std::string& name) : Layer(name) {}

		SceneViewportPanel* m_Viewport = nullptr;

		bool m_WindowMaximized = true;

		bool m_OpenNewSceneModal = false;

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

		void ReloadCSharpScripts();

	private:
		Eventing::ListenerID m_WindowKeyPressedListener;
	};

} // namespace SW
