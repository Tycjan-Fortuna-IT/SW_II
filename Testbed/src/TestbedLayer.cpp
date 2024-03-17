#include "TestbedLayer.hpp"

#include "Core/Renderer/RendererAPI.hpp"
#include "gui.hpp"
#include "AssetPanels/AssetEditorPanelManager.hpp"
#include "Core/Asset/Asset.hpp"
#include "Core/Project/ProjectSerializer.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"

namespace SW {

	class Spritesheet final : public Asset
	{
		AssetType GetAssetType() const override { return AssetType::Spritesheet; }
	};

	static Spritesheet* sp = new Spritesheet();

	void TestbedLayer::OnAttach()
	{
		const GUI::FontSpecification fontSpec("assets/fonts/Roboto/Roboto-Regular.ttf", "assets/fonts/Roboto/Roboto-Bold.ttf");

		GUI::Appearance::ApplyStyle(GUI::Style()); 
		GUI::Appearance::ApplyColors(GUI::Colors());
		GUI::Appearance::ApplyFonts(fontSpec);

		Application::Get()->GetWindow()->SetVSync(true);

		Renderer2D::Initialize();

		AssetEditorPanelManager::Initialize();

		Project* newProject = ProjectSerializer::Deserialize("C:\\Users\\tycja\\Desktop\\SW_II\\Testbed\\Testbed.swproj");
		ProjectContext::Set(newProject); // TODO: Make projects switchable
	}

	void TestbedLayer::OnDetach()
	{
		Renderer2D::Shutdown();
		AssetEditorPanelManager::Shutdown();
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
