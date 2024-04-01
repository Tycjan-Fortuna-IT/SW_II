#include "TestbedLayer.hpp"

#include "Core/Renderer/RendererAPI.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Project/ProjectSerializer.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Core/Asset/AssetManager.hpp"
#include "../../EngineEditor/src/AssetPanels/AssetEditorPanelManager.cpp"
#include "../../EngineEditor/src/AssetPanels/AssetEditorPanelManager.hpp"
#include "../../EngineEditor/src/AssetPanels/SpritesheetEditor.cpp"
#include "../../EngineEditor/src/AssetPanels/SpritesheetEditor.hpp"

#include "../../EngineEditor/src/Panels/AssetPanel.cpp"
#include "../../EngineEditor/src/Panels/AssetPanel.hpp"
#include <future>
#include "GUI/Editor/EditorResources.hpp"
#include "Core/OpenGL/Texture2D.hpp"
#include "Core/Asset/Spritesheet.hpp"

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
		//auto temp = std::filesystem::last_write_time("C:\\Users\\tycja\\Desktop\\SW_II\\Testbed\\Testbed.swproj");
		//u64 val = temp.time_since_epoch().count();

		//std::string time = std::format("File write time is {}\n", temp);
		///File write time is 2024-03-17 20:04:54.9173268
		//SW_ERROR("{}", val);

		assetPanel = new AssetPanel();

		newProject = ProjectSerializer::Deserialize("C:\\Users\\tycja\\Desktop\\SW_II\\Testbed\\Testbed.swproj");
		ProjectContext::Set(newProject); // TODO: Make projects switchable

		// sp = new Spritesheet(Random::CreateID(), AssetManager::GetTexture2D("assets\\spritesheets\\spritesheet_test.png"));

		// Spritesheet* test = sp->As<Spritesheet>();
		// SW_TRACE("{}", test->GetSpritesheetTexture()->GetHeight());

		//const Texture2D* triangle = AssetManager::GetAsset<Texture2D>(2560501347489892845);

		SW_TRACE("1");
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
		GUI::CreateDockspace("Main Dockspace", nullptr);

		AssetEditorPanelManager::OnRender();

		assetPanel->OnRender();

		ImGui::ShowDemoWindow();
	}

}
