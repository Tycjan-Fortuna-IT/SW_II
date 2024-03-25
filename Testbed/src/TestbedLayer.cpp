#include "TestbedLayer.hpp"

#include "Core/Renderer/RendererAPI.hpp"
#include "gui.hpp"
#include "AssetPanels/AssetEditorPanelManager.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Project/ProjectSerializer.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "AssetPanels/SpritesheetEditor.hpp"
#include "Core/Asset/AssetLoader.hpp"

namespace SW {

	static Asset* sp = nullptr;

	void TestbedLayer::OnAttach()
	{
		const GUI::FontSpecification fontSpec("assets/fonts/Roboto/Roboto-Regular.ttf", "assets/fonts/Roboto/Roboto-Bold.ttf");

		GUI::Appearance::ApplyStyle(GUI::Style()); 
		GUI::Appearance::ApplyColors(GUI::Colors());
		GUI::Appearance::ApplyFonts(fontSpec);

		Application::Get()->GetWindow()->SetVSync(true);

		Renderer2D::Initialize();

		AssetEditorPanelManager::Initialize();
		AssetLoader::Initialize();

		//auto temp = std::filesystem::last_write_time("C:\\Users\\tycja\\Desktop\\SW_II\\Testbed\\Testbed.swproj");
		//u64 val = temp.time_since_epoch().count();

		//std::string time = std::format("File write time is {}\n", temp);
		///File write time is 2024-03-17 20:04:54.9173268
		//SW_ERROR("{}", val);

		Project* newProject = ProjectSerializer::Deserialize("C:\\Users\\tycja\\Desktop\\SW_II\\Testbed\\Testbed.swproj");
		ProjectContext::Set(newProject); // TODO: Make projects switchable

		sp = new Spritesheet(AssetManager::GetTexture2D("assets\\spritesheets\\spritesheet_test.png"));

		Spritesheet* test = sp->As<Spritesheet>();
		SW_TRACE("{}", test->GetSpritesheetTexture()->GetHeight());
	}

	void TestbedLayer::OnDetach()
	{
		Renderer2D::Shutdown();
		AssetEditorPanelManager::Shutdown();
		AssetLoader::Shutdown();
	}

	void TestbedLayer::OnUpdate(Timestep dt)
	{
		RendererAPI::Clear();
		AssetEditorPanelManager::OnUpdate(dt);
	}

	static void OpenPanel()
	{
		AssetEditorPanelManager::OpenEditor(sp);
	}

	static void ClosePanel()
	{

	}

	void TestbedLayer::OnRender()
	{
		GUI::CreateDockspace("Main Dockspace", nullptr);

		AssetEditorPanelManager::OnRender();

		ImGui::Begin("Panel");

		if (ImGui::Button("Open panel")) {
			OpenPanel();
		}
		
		if (ImGui::Button("Close panel")) {
			ClosePanel();
		}

		ImGui::End();

		ImGui::ShowDemoWindow();
	}

}
