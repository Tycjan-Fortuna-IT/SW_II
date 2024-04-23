#include "TestbedLayer.hpp"

#include "Core/Renderer/RendererAPI.hpp"
#include "Asset/Asset.hpp"
#include "Core/Project/ProjectSerializer.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Asset/AssetManager.hpp"
#include "GUI/Editor/EditorResources.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Asset/Spritesheet.hpp"

#include "../../EngineEditor/src/AssetPanels/AssetEditorPanelManager.cpp"
#include "../../EngineEditor/src/AssetPanels/AssetEditorPanelManager.hpp"
#include "../../EngineEditor/src/AssetPanels/SpritesheetEditor.cpp"
#include "../../EngineEditor/src/AssetPanels/SpritesheetEditor.hpp"
#include "../../EngineEditor/src/AssetPanels/AnimationEditor.cpp"
#include "../../EngineEditor/src/AssetPanels/AnimationEditor.hpp"
#include "../../EngineEditor/src/Panels/AssetPanel.cpp"
#include "../../EngineEditor/src/Panels/AssetPanel.hpp"
#include "Core/Utils/FileSystem.hpp"
#include "../GUI_V2.hpp"
#include "imgui_combo_autoselect.hpp"

namespace SW {

	static AssetPanel* assetPanel = nullptr;
	static Project* newProject = nullptr;

	void TestbedLayer::OnAttach()
	{
		const GUI::FontSpecification fontSpec("assets/fonts/Roboto/Roboto-Regular.ttf", "assets/fonts/Roboto/Roboto-Bold.ttf");

		GUI::Appearance::ApplyStyle(GUI::Style()); 
		GUI::Appearance::ApplyColors(GUI::Colors());
		GUI::Appearance::ApplyFonts(fontSpec);

		Application::Get()->GetWindow()->SetVSync(true);

		Renderer2D::Initialize();
		EditorResources::Initialize();

		AssetEditorPanelManager::Initialize();
		AssetManager::Initialize();
		assetPanel = new AssetPanel();

		newProject = ProjectSerializer::Deserialize(R"(C:\Users\tycja\Desktop\SW_II\Testbed\Testbed.swproj)");
		ProjectContext::Set(newProject); // TODO: Make projects switchable
	}

	void TestbedLayer::OnDetach()
	{
		Renderer2D::Shutdown();
		EditorResources::Shutdown();

		AssetEditorPanelManager::Shutdown();
		AssetManager::Shutdown();

		delete assetPanel;
		delete newProject;
	}

	void TestbedLayer::OnUpdate(Timestep dt)
	{
		RendererAPI::Clear();
		AssetEditorPanelManager::OnUpdate(dt);
		assetPanel->OnUpdate(dt);
	}

	void TestbedLayer::OnRender()
	{
		GUI2::Layout::CreateDockspace("Main Dockspace", nullptr);

		AssetEditorPanelManager::OnRender();

		assetPanel->OnRender();

		ImGui::Begin("New GUI API");
		static std::string search = "";
		static std::string search2 = "";

		ImGui::PushID("asdasd");
		GUI2::Widgets::SearchInput<64>(&search);
		ImGui::PopID();

		ImGui::NewLine();

		ImGui::PushID("asdasd1");
		GUI2::Widgets::SearchInput<64>(&search2);
		ImGui::PopID();

		//ImGui::TextUnformatted(search.c_str());
		//static bool val = false;
		//GUI_V2::ToggleButton("Test", &val);
		//GUI_V2::ToggleButton2("Test", &val);
		//ImGui::Checkbox("s", &val);

		//std::string text = "jksfdjkfj";
		//Display(text);

		GUI2::Components::Text("POP - POP, Inernal {}", 1);
		GUI2::Components::ItemOutline();


		ImGui::End();

		ImGui::ShowDemoWindow();
	}

}
