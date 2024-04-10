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

		newProject = ProjectSerializer::Deserialize("C:\\Users\\tycja\\Desktop\\SW_II\\Testbed\\Testbed.swproj");
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
		GUI::CreateDockspace("Main Dockspace", nullptr);

		AssetEditorPanelManager::OnRender();

		assetPanel->OnRender();

		ImGui::ShowDemoWindow();
	}

}
