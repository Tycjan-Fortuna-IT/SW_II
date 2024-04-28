#include "TestbedLayer.hpp"

#include "Core/Renderer/RendererAPI.hpp"
#include "Core/Project/ProjectSerializer.hpp"
#include "Core/Project/Project.hpp"
#include "Core/Project/ProjectContext.hpp"
#include "Asset/AssetManager.hpp"
#include "GUI/Editor/EditorResources.hpp"
#include "../../EngineEditor/src/AssetPanels/AssetEditorPanelManager.cpp"
#include "../../EngineEditor/src/AssetPanels/AssetEditorPanelManager.hpp"
#include "../../EngineEditor/src/AssetPanels/SpritesheetEditor.cpp"
#include "../../EngineEditor/src/AssetPanels/SpritesheetEditor.hpp"
#include "../../EngineEditor/src/AssetPanels/AnimationEditor.cpp"
#include "../../EngineEditor/src/AssetPanels/AnimationEditor.hpp"
#include "../../EngineEditor/src/Panels/AssetPanel.cpp"
#include "../../EngineEditor/src/Panels/AssetPanel.hpp"
#include "Core/Utils/FileSystem.hpp"
#include "GUI_V2.hpp"
#include "imgui_combo_autoselect.hpp"

namespace SW {

	static AssetPanel* assetPanel = nullptr;
	static Project* newProject = nullptr;
	static Texture2D* texture = nullptr;

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

		texture = *AssetManager::GetAssetRaw<Texture2D>(17232749685889010059);
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
#if 0
		static std::string search;
		static std::string search2;
		static std::string search3;
		static std::string search4;

		{
			GUI2::ScopedID id(1);
			GUI2::Widgets::SearchInput<64>(&search);
		}

		{
			GUI2::ScopedID id(2);
			GUI2::Components::SingleLineTextInput<64>(&search2);
		}

		{
			GUI2::ScopedID id(3);
			GUI2::Components::SingleLineTextInputDeffered<64>(&search3);
		}

		{
			GUI2::ScopedID id(4);
			GUI2::Components::MultiLineTextInput<64>(&search4);
		}

		{
			GUI2::ScopedID id(5);
			GUI2::Components::MultiLineTextInputDeffered<64>(&search4);
		}

		{
			GUI2::ScopedID id(6);
			static bool val = false;
			ImGui::PushMultiItemsWidths(2, ImGui::GetContentRegionAvail().x);
			GUI2::Components::Checkbox(&val, false);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			GUI2::Components::Checkbox(&val, false);
			ImGui::PopItemWidth();
		}

		{
			GUI2::ScopedID id(7);
			static bool val = false;
			GUI2::Components::Toggle(&val, false);
		}

		{
			GUI2::ScopedID id(8);
			static bool val = false;
		}

		{
			GUI2::ScopedID id(9);
			static int val = 0;
			GUI2::Components::RadioButton(&val, {
				{ "Radio1", 0 }, { "Radio1", 0 }
			}, false);
			GUI2::Components::RadioButton(&val, {
				{ "Raddsfdfsfsdio1", 0 }, { "Radio2", 1 }, { "Radio3", 2 }, { "Radio4", 3 }
			}, false);
			GUI2::Components::RadioButton(&val, {
				{ "Radio1", 0 }, { "Radio2", 1 }, { "Radisdfsdfo3", 2 }, { "Radio4", 3 }, { "Radio5", 4 }
			}, false);
		}

		{
			GUI2::ScopedID id(10);
			static int val = 0;
			GUI2::Components::ScalarInput<int>(&val, 1, 10);
		}

		{
			GUI2::ScopedID id(11);
			static f32 val = 0.0f;
			GUI2::Components::ScalarInput<f32>(&val, 1, 10);
		}

		{
			GUI2::ScopedID id(12);
			static f64 val = 0.0;
			GUI2::Components::ScalarInput<f64>(&val, 1, 10);
		}

		{
			GUI2::ScopedID id(13);
			static int val = 0;
			GUI2::Components::ScalarSlider<int>(&val, 1, 10);
		}

		{
			GUI2::ScopedID id(14);
			static f32 val = 0;
			GUI2::Components::ScalarSlider<f32>(&val, 1, 10);
		}

		{
			GUI2::ScopedID id(15);
			static f64 val = 0;
			GUI2::Components::ScalarSlider<f64>(&val, 1, 10);
		}

		{
			GUI2::ScopedID id(16);
			static int val = 0;
			GUI2::Components::ScalarDrag<int>(&val, 2.f, 1, 100);
		}

		{
			GUI2::ScopedID id(17);
			static f32 val = 0;
			GUI2::Components::ScalarDrag<f32>(&val, 2.f, 1, 100);
		}

		{
			GUI2::ScopedID id(18);
			static f64 val = 0;
			GUI2::Components::ScalarDrag<f64>(&val, 2.f, 1, 100);
		}

		{
			GUI2::ScopedID id(19);
			static glm::vec2 val = { 0, 0 };
			GUI2::Components::Vector2Input(&val, 1.f, -30.f, 30.f);
		}

		{
			GUI2::ScopedID id(20);
			static glm::vec3 val = { 0, 0, 0 };
			GUI2::Components::Vector3Input(&val, 1.f, -30.f, 30.f);
		}

		{
			GUI2::ScopedID id(21);
			static glm::vec3 val = { 0, 0, 0 };
			GUI2::Components::Vector3ColorPicker(&val);
		}

		{
			GUI2::ScopedID id(22);
			static glm::vec4 val = { 0, 0, 0, 0 };
			GUI2::Components::Vector4ColorPicker(&val);
		}

		{
			GUI2::ScopedID id(23);
			static int val = 0;
			GUI2::Components::Selectable(&val, {
				{ "Radio1", 0 }, { "Radio2", 1 }, { "Radisdfsdfo3", 2 }, { "Radio4", 3 }, { "Radio5", 4 }
			}, false);
		}

		{
			GUI2::ScopedID id(24);
			static bool val = false;
			GUI2::Components::ToggleButton(&val, SW_ICON_CAMERA "ON", SW_ICON_ANGLE_ACUTE "OFF");
		}

		{
			GUI2::ScopedID id(25);
			static bool val = false;
			GUI2::Components::ImageButton(GUI2::GetTextureID(texture), { 64.f, 64.f });
		}

		GUI2::Components::Text(search2);
		GUI2::Components::Text(search3);

		GUI2::Components::Text("POP - POP, Inernal {}", 1);
		GUI2::Components::ItemOutline();
		ImGui::NewLine();

		{
			GUI2::ScopedID id(26);
			static std::filesystem::path val;
			GUI2::Widgets::DrawFolderPickerProperty(&val, ProjectContext::Get()->GetAssetDirectory());
		}

		ImGui::NewLine();


		{
			GUI2::ScopedID id(27);
			static std::filesystem::path val;
			GUI2::Widgets::DrawFilePickerProperty(&val, ProjectContext::Get()->GetAssetDirectory(), {{ "Log file", "log" }});
		}

		{
			GUI2::ScopedID id(27);
			static std::vector<glm::vec2> val = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };
			GUI2::Widgets::Vector2Table(&val, 1.f);
		}

		{
			GUI2::ScopedID id(28);
			static std::vector<glm::vec3> val = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
			GUI2::Widgets::Vector3Table(&val, 1.f);
		}
#endif // 0

		{
			GUI2::ScopedID id(29);
			static AssetHandle handle = 0;
			GUI2::Widgets::DrawAssetDropdownProperty<Texture2D>(&handle);
		}

		ImGui::End();

		ImGui::ShowDemoWindow();
	}

}
