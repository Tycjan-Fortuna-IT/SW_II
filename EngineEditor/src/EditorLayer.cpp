#include "EditorLayer.hpp"

#include "Panels/StatisticsPanel.hpp"
#include "Panels/ConsolePanel.hpp"
#include "Panels/AssetPanel.hpp"
#include "Panels/SceneHierarchyPanel.hpp"
#include "Panels/PropertiesPanel.hpp"
#include "Core/Scene/SceneSerializer.hpp"
#include "Core/Utils/FileSystem.hpp"
#include "Managers/SelectionManager.hpp"
#include "Panels/SceneViewportPanel.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectSerializer.hpp"
#include "Core/Scripting/ScriptingCore.hpp"
#include "GUI/Editor/EditorResources.hpp"
#include "AssetPanels/AssetEditorPanelManager.hpp"
#include "GUI/GUI.hpp"

namespace SW {

	void EditorLayer::OnAttach()
	{
		EditorResources::Initialize();
		AssetEditorPanelManager::Initialize();

		const GUI::FontSpecification fontSpec("assets/fonts/Roboto/Roboto-Regular.ttf", "assets/fonts/Roboto/Roboto-Bold.ttf");

		GUI::Appearance::ApplyStyle(GUI::Style());
		GUI::Appearance::ApplyColors(GUI::Colors());
		GUI::Appearance::ApplyFonts(fontSpec);

		EventSystem::Register(EVENT_CODE_KEY_PRESSED, [this](Event event) -> bool {
			KeyCode code = (KeyCode)event.Payload.u16[0];

			return OnKeyPressed(code);
		});

		m_Panels.emplace_back(new StatisticsPanel("Statistics"));
		m_Panels.emplace_back(new ConsolePanel());
		m_Panels.emplace_back(new AssetPanel());

		m_Viewport = new SceneViewportPanel();
		
		m_Panels.emplace_back(m_Viewport);
		m_Panels.emplace_back(new PropertiesPanel(m_Viewport));
		m_Panels.emplace_back(new SceneHierarchyPanel(m_Viewport));

		Application::Get()->GetWindow()->SetVSync(true);

		Renderer2D::Initialize();

#ifdef SW_DEBUG_BUILD
		OpenProject();
#endif
	}

	void EditorLayer::OnDetach()
	{
		for (Panel* panel : m_Panels) {
			delete panel;
		}

		EditorResources::Shutdown();
		AssetEditorPanelManager::Shutdown();

		Renderer2D::Shutdown();

		if (ProjectContext::HasContext())
			delete ProjectContext::Get();
	}

	void EditorLayer::OnUpdate(Timestep dt)
	{
		PROFILE_FUNCTION();

		for (Panel* panel : m_Panels) {
			if (panel->IsShowing())
				panel->OnUpdate(dt);
		}

		AssetEditorPanelManager::OnUpdate(dt);
	}

	f32 EditorLayer::DrawTitleBar()
	{
		PROFILE_FUNCTION();

		constexpr f32 titlebarHeight = 57.0f;
		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

		ImGui::SetCursorPos(windowPadding);

		const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
		const ImVec2 titlebarMax = {
			ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
			ImGui::GetCursorScreenPos().y + titlebarHeight
		};

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->AddRectFilled(titlebarMin, titlebarMax, Color::TitleBar);
		ImU32 gradientColor = IM_COL32(91, 55, 55, 255);

		if (m_Viewport->IsSceneLoaded()) {
			switch(m_Viewport->GetCurrentScene()->GetCurrentState()) {
				case SceneState::Edit:
					gradientColor = Color::LightBlack;
					break;
				case SceneState::Play:
					gradientColor = IM_COL32(55, 91, 71, 255);
					break;
				case SceneState::Pause:
					gradientColor = IM_COL32(91, 91, 55, 255);
					break;
			}
		}
		
		drawList->AddRectFilledMultiColor(titlebarMin, ImVec2(titlebarMin.x + 600.0f, titlebarMax.y), gradientColor, Color::TitleBar, Color::TitleBar, gradientColor);

		{
			const f32 logoWidth = (f32)EditorResources::SW_Icon->GetWidth() / 3.f;
			const f32 logoHeight = (f32)EditorResources::SW_Icon->GetHeight() / 3.f;
			const ImVec2 logoOffset(20.0f + windowPadding.x, windowPadding.y - 1.2f);
			const ImVec2 logoRectStart = { ImGui::GetItemRectMin().x + logoOffset.x, ImGui::GetItemRectMin().y + logoOffset.y };
			const ImVec2 logoRectMax = { logoRectStart.x + logoWidth, logoRectStart.y + logoHeight };

			drawList->AddImage(GUI::GetTextureID(EditorResources::SW_Icon), logoRectStart, logoRectMax, { 0, 1 }, { 1, 0 });
		}

		const f32 w = ImGui::GetContentRegionAvail().x;
		constexpr f32 buttonsAreaWidth = 350; // just so it doesn't interfere with buttons on the right

		ImGui::SetCursorPosX(230); // just so it doesn't interfere with the menubar on the left
		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight));

		if (ImGui::IsItemHovered()) {
			Application::Get()->GetWindow()->RegisterOverTitlebar(true);
		}

		{
			const f32 logoOffset = 16.0f * 2.0f + 41.0f + windowPadding.x;
			ImGui::SetCursorPos(ImVec2(logoOffset, 4.0f));

			DrawMenuBar();
		}

		{
			if (ProjectContext::Get()) {
				GUI::ScopedColor TextColor(ImGuiCol_Text, Color::TextDarker);
				GUI::ScopedColor BorderColor(ImGuiCol_Border, Color::BorderDarker);

				const std::string projectName = ProjectContext::Get()->GetConfig().Name;
				const ImVec2 textSize = ImGui::CalcTextSize(projectName.c_str());

				const f32 rightOffset = ImGui::GetWindowWidth() / 2.f - 70.f;

				ImGui::SameLine();
				ImGui::SetCursorPosX(rightOffset - windowPadding.x);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f + windowPadding.y);

				ImGui::Text(projectName.c_str());

				GUI::Components::RectangleOutline(GUI::RectExpanded(GUI::GetItemRect(), 24.0f, 68.0f), GUI::Theme::Outline,
					1.0f, 3.0f, 0.0f, -60.0f);
			}
		}

		{
			GUI::ScopedColor TextColor(ImGuiCol_Text, Color::TextDarker);
			GUI::ScopedColor BorderColor(ImGuiCol_Border, Color::BorderDarker);

			const std::string memoryText = "MEM: " + String::GetAllocatedMemoryString();
			const ImVec2 textSize = ImGui::CalcTextSize(memoryText.c_str());

			const f32 rightOffset = ImGui::GetWindowWidth() - 280.0f;
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(rightOffset - windowPadding.x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f + windowPadding.y);
			ImGui::Text(memoryText.c_str());

			GUI::Components::RectangleOutline(GUI::RectExpanded(GUI::GetItemRect(), 24.0f, 68.0f), GUI::Theme::Outline, 
				1.0f, 3.0f, 0.0f, -60.0f);

			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::Text("Memory usage: %s b", String::GetAllocatedMemoryStringRaw().c_str());
				ImGui::EndTooltip();
			}
		}

		// Minimize Button
		{
			const f32 rightOffset = ImGui::GetWindowWidth() - 120.0f;
			ImGui::SameLine();
			ImGui::SetCursorPosX(rightOffset - windowPadding.x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.0f + windowPadding.y);
			ImGui::SetItemAllowOverlap();

			if (GUI::Components::ImageButton(*EditorResources::MinimizeIcon, { 40.f, 40.f })) {
				Application::Get()->GetWindow()->Minimize();
			}
		}

		// Maximize / shrink button
		{
			const f32 rightOffset = ImGui::GetWindowWidth() - 80.0f;
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(rightOffset - windowPadding.x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.0f + windowPadding.y);
			ImGui::SetItemAllowOverlap();

			if (m_WindowMaximized) {
				if (GUI::Components::ImageButton(*EditorResources::RestoreIcon, { 40.f, 40.f })) {
					Application::Get()->GetWindow()->Restore();
				}
			} else {
				if (GUI::Components::ImageButton(*EditorResources::MaximizeIcon, { 40.f, 40.f })) {
					Application::Get()->GetWindow()->Maximize();
				}
			}
		}

		// Close button
		{
			const f32 rightOffset = ImGui::GetWindowWidth() - 40.0f;
			ImGui::SameLine();
			ImGui::SetCursorPosX(rightOffset - windowPadding.x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.0f + windowPadding.y);
			ImGui::SetItemAllowOverlap();

			if (GUI::Components::ImageButton(*EditorResources::CloseIcon, { 40.f, 40.f })) {
				Application::Get()->Close();
			}
		}

		return titlebarHeight;
	}

	void EditorLayer::DrawMenuBar()
	{
		PROFILE_FUNCTION();

		ImGui::SetCursorPosX(120); // just so it doesn't interfere with the SW logo

		const ImRect menuBarRect = { 
			ImGui::GetCursorPos(), { ImGui::GetContentRegionAvail().x + ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing() }
		};

		ImGui::BeginGroup();

		if (GUI::Layout::BeginMenuBar(menuBarRect)) {
			GUI::ScopedColor HeaderColor(ImGuiCol_Header, Color::DarkGray);
			GUI::ScopedColor HeaderHoveredColor(ImGuiCol_HeaderHovered, Color::DarkGray);

			if (ImGui::BeginMenu("File")) {
				if (ProjectContext::HasContext()) { // project must be opened to create new scene
					if (ImGui::MenuItem("Create New Scene", "Ctrl+N")) {
						CreateNewScene();
					}
				}

				if (ImGui::MenuItem("Open Project", "Ctrl+O")) {
					OpenProject();
				}

				if (m_Viewport->IsSceneLoaded()) {
					if (ImGui::MenuItem("Save Current Scene", "Ctrl+S")) {
						SaveCurrentScene();
					}
				}

				if (ProjectContext::HasContext()) {
					if (ImGui::MenuItem("Save Project", "Ctrl+Shift+S")) {
						SaveProjectAs();
					}
				}

				if (ImGui::MenuItem("Close Editor", "Esc")) {
					Application::Get()->Close();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scripting")) {
				if (ImGui::MenuItem("Reload C# Assemblies", "Ctrl+B")) {
					ReloadCSharpScripts();
				}

				ImGui::EndMenu();
			}
		}
		GUI::Layout::EndMenuBar();


		ImGui::EndGroup();
	}

	void EditorLayer::OnRender()
	{
		PROFILE_FUNCTION();

		Window* window = Application::Get()->GetWindow();

		window->RegisterOverTitlebar(false);
		m_WindowMaximized = window->IsCurrentlyMaximized();
		
		GUI::Layout::CreateDockspace("Main dockspace", [this]() -> f32 {
			return DrawTitleBar();
		});

		for (Panel* panel : m_Panels) {
			if (panel->IsShowing())
				panel->OnRender();
		}

		AssetEditorPanelManager::OnRender();

		if (m_OpenNewSceneModal) {
			m_OpenNewSceneModal = false;
			m_IsNewSceneModalOpen = true;

			ImGui::OpenPopup("NewSceneModal");
		}

		//ImGui::ShowStyleEditor();

		if (ImGui::BeginPopupModal("NewSceneModal", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Input the desired scene name with .sw extension!");

			ImGui::Separator();

			static std::string newProjectName;

			GUI::Properties::BeginProperties("##new_scene_name");
			GUI::Properties::SingleLineTextInputProperty<128>(&newProjectName, "Name");
			GUI::Properties::EndProperties();

			if (!newProjectName.empty()) { // TODO fixme - NEW BETTER POPUP
				if (ImGui::Button("Create", ImVec2(100.f, 0))) {
					if (SelectionManager::IsSelected())
						SelectionManager::Deselect();

					Scene* newScene = new Scene();

					// TODO
					SceneSerializer::Serialize(newScene, ProjectContext::Get()->GetAssetDirectory() / "scenes" / newProjectName);

					delete m_Viewport->GetCurrentScene();
					m_Viewport->SetCurrentScene(newScene);

					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();
			}

			if (ImGui::Button("Cancel", ImVec2(100.f, 0))) {
				m_IsNewSceneModalOpen = false;

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	bool EditorLayer::OnKeyPressed(KeyCode code)
	{
		const bool shift = Input::IsKeyDown(KeyCode::LeftShift) || Input::IsKeyDown(KeyCode::RightShift);
		const bool ctrl = Input::IsKeyDown(KeyCode::LeftControl) || Input::IsKeyDown(KeyCode::RightControl);
		//const bool alt = Input::IsKeyDown(KeyCode::LeftAlt) || Input::IsKeyDown(KeyCode::RightAlt);

		switch (code) {
			case KeyCode::Escape:
				if (!m_IsNewSceneModalOpen)
					Application::Get()->Close(); break;
			case KeyCode::S:
				if (ctrl && shift && ProjectContext::HasContext()) {
					SaveProjectAs(); break;
				} else if (ctrl && m_Viewport->IsSceneLoaded()) {
					SaveCurrentScene(); break;
				}
			case KeyCode::O:
				if (ctrl) OpenProject(); break;
			case KeyCode::N:
				if (ctrl && ProjectContext::HasContext()) CreateNewScene(); break;
			case KeyCode::B:
				if (ctrl) ReloadCSharpScripts(); break;
			default:
				break;
		}

		return false;
	}

	void EditorLayer::CreateNewScene()
	{
		m_OpenNewSceneModal = true;
	}

	void EditorLayer::SaveCurrentScene()
	{
		Scene* currentScene = m_Viewport->GetCurrentScene();

		const AssetMetaData& metadata = AssetManager::GetAssetMetaData(currentScene->GetHandle());
		//AssetLoader::Serialize(metadata);// reload after ?
		SceneSerializer::Serialize(currentScene, ProjectContext::Get()->GetAssetDirectory() / metadata.Path);
	}

	void EditorLayer::OpenProject()
	{
		std::filesystem::path filepath = FileSystem::OpenFileDialog({ { "SW Engine Scene file", "swproj" } });

		if (!filepath.empty()) {
			if (SelectionManager::IsSelected())
				SelectionManager::Deselect();

			Project* currentProject = ProjectContext::Get();
			if (currentProject)
				ScriptingCore::Get().Shutdown();

			delete currentProject;

			std::string path = filepath.string();

			Project* newProject = ProjectSerializer::Deserialize(path);
			ProjectContext::Set(newProject); // TODO: Make projects switchable

			ScriptingCore::Get().Initialize();
		}
	}

	void EditorLayer::SaveProjectAs()
	{
		if (!ProjectContext::HasContext())
			return;

		std::filesystem::path filepath = FileSystem::SaveFileDialog({ { "SW Engine Scene file", "swproj" } });

		if (!filepath.empty()) {
			ProjectSerializer::Serialize(ProjectContext::Get(), filepath.string());

			Scene* currentScene = m_Viewport->GetCurrentScene();

			const AssetMetaData& metadata = AssetManager::GetAssetMetaData(currentScene->GetHandle());

			SceneSerializer::Serialize(currentScene, ProjectContext::Get()->GetAssetDirectory() / metadata.Path);
		}
	}

	void EditorLayer::ReloadCSharpScripts()
	{
		if (!ProjectContext::HasContext())
			return;

		if (m_Viewport->GetCurrentScene()->GetCurrentState() != SceneState::Edit)
			return;

		SW_INFO("Reloading C# Scripts ...");

		ScriptingCore& core = ScriptingCore::Get();

		ScriptStorage tempStorage;

		auto& scriptStorage = m_Viewport->GetCurrentScene()->GetScriptStorage();
		scriptStorage.CopyTo(tempStorage);
		scriptStorage.Clear();

		core.Shutdown();
		core.Initialize();

		tempStorage.CopyTo(scriptStorage);
		tempStorage.Clear();

		scriptStorage.SynchronizeStorage();
	}

}
