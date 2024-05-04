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
#include "GUI/GUI.hpp"

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
		GUI::Layout::CreateDockspace("Main Dockspace", nullptr);

		AssetEditorPanelManager::OnRender();

		assetPanel->OnRender();

		//ImGui::Begin("New GUI API");
#if 0
		static std::string search;
		static std::string search2;
		static std::string search3;
		static std::string search4;

		{
			GUI::ScopedID id(1);
			GUI::Widgets::SearchInput<64>(&search);
		}

		{
			GUI::ScopedID id(2);
			GUI::Components::SingleLineTextInput<64>(&search2);
		}

		{
			GUI::ScopedID id(3);
			GUI::Components::SingleLineTextInputDeffered<64>(&search3);
		}

		{
			GUI::ScopedID id(4);
			GUI::Components::MultiLineTextInput<64>(&search4);
		}

		{
			GUI::ScopedID id(5);
			GUI::Components::MultiLineTextInputDeffered<64>(&search4);
		}

		{
			GUI::ScopedID id(6);
			static bool val = false;
			ImGui::PushMultiItemsWidths(2, ImGui::GetContentRegionAvail().x);
			GUI::Components::Checkbox(&val, false);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			GUI::Components::Checkbox(&val, false);
			ImGui::PopItemWidth();
		}

		{
			GUI::ScopedID id(7);
			static bool val = false;
			GUI::Components::Toggle(&val, false);
		}

		{
			GUI::ScopedID id(8);
			static bool val = false;
		}

		{
			GUI::ScopedID id(9);
			static int val = 0;
			GUI::Components::RadioButton(&val, {
				{ "Radio1", 0 }, { "Radio1", 0 }
			}, false);
			GUI::Components::RadioButton(&val, {
				{ "Raddsfdfsfsdio1", 0 }, { "Radio2", 1 }, { "Radio3", 2 }, { "Radio4", 3 }
			}, false);
			GUI::Components::RadioButton(&val, {
				{ "Radio1", 0 }, { "Radio2", 1 }, { "Radisdfsdfo3", 2 }, { "Radio4", 3 }, { "Radio5", 4 }
			}, false);
		}

		{
			GUI::ScopedID id(10);
			static int val = 0;
			GUI::Components::ScalarInput<int>(&val, 1, 10);
		}

		{
			GUI::ScopedID id(11);
			static f32 val = 0.0f;
			GUI::Components::ScalarInput<f32>(&val, 1, 10);
		}

		{
			GUI::ScopedID id(12);
			static f64 val = 0.0;
			GUI::Components::ScalarInput<f64>(&val, 1, 10);
		}

		{
			GUI::ScopedID id(13);
			static int val = 0;
			GUI::Components::ScalarSlider<int>(&val, 1, 10);
		}

		{
			GUI::ScopedID id(14);
			static f32 val = 0;
			GUI::Components::ScalarSlider<f32>(&val, 1, 10);
		}

		{
			GUI::ScopedID id(15);
			static f64 val = 0;
			GUI::Components::ScalarSlider<f64>(&val, 1, 10);
		}

		{
			GUI::ScopedID id(16);
			static int val = 0;
			GUI::Components::ScalarDrag<int>(&val, 2.f, 1, 100);
		}

		{
			GUI::ScopedID id(17);
			static f32 val = 0;
			GUI::Components::ScalarDrag<f32>(&val, 2.f, 1, 100);
		}

		{
			GUI::ScopedID id(18);
			static f64 val = 0;
			GUI::Components::ScalarDrag<f64>(&val, 2.f, 1, 100);
		}

		{
			GUI::ScopedID id(19);
			static glm::vec2 val = { 0, 0 };
			GUI::Components::Vector2Input(&val, 1.f, -30.f, 30.f);
		}

		{
			GUI::ScopedID id(20);
			static glm::vec3 val = { 0, 0, 0 };
			GUI::Components::Vector3Input(&val, 1.f, -30.f, 30.f);
		}

		{
			GUI::ScopedID id(21);
			static glm::vec3 val = { 0, 0, 0 };
			GUI::Components::Vector3ColorPicker(&val);
		}

		{
			GUI::ScopedID id(22);
			static glm::vec4 val = { 0, 0, 0, 0 };
			GUI::Components::Vector4ColorPicker(&val);
		}

		{
			GUI::ScopedID id(23);
			static int val = 0;
			GUI::Components::Selectable(&val, {
				{ "Radio1", 0 }, { "Radio2", 1 }, { "Radisdfsdfo3", 2 }, { "Radio4", 3 }, { "Radio5", 4 }
			}, false);
		}

		{
			GUI::ScopedID id(24);
			static bool val = false;
			GUI::Components::ToggleButton(&val, SW_ICON_CAMERA "ON", SW_ICON_ANGLE_ACUTE "OFF");
		}

		{
			GUI::ScopedID id(25);
			static bool val = false;
			GUI::Components::ImageButton(GUI::GetTextureID(texture), { 64.f, 64.f });
		}

		GUI::Components::Text(search2);
		GUI::Components::Text(search3);

		GUI::Components::Text("POP - POP, Inernal {}", 1);
		GUI::Components::ItemOutline();
		ImGui::NewLine();

		{
			GUI::ScopedID id(26);
			static std::filesystem::path val;
			GUI::Widgets::DrawFolderPicker(&val, ProjectContext::Get()->GetAssetDirectory());
		}

		ImGui::NewLine();


		{
			GUI::ScopedID id(27);
			static std::filesystem::path val;
			GUI::Widgets::DrawFilePicker(&val, ProjectContext::Get()->GetAssetDirectory(), {{ "Log file", "log" }});
		}

		{
			GUI::ScopedID id(27);
			static std::vector<glm::vec2> val = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };
			GUI::Widgets::Vector2Table(&val, 1.f);
		}

		{
			GUI::ScopedID id(28);
			static std::vector<glm::vec3> val = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
			GUI::Widgets::Vector3Table(&val, 1.f);
		}
		{
			GUI::ScopedID id(29);
			static AssetHandle handle = 0;
			GUI::Widgets::AssetSearch<Texture2D>(&handle);
		}

		{
			GUI::ScopedID id(30);
			static std::vector<Texture2D**> textures;
			GUI::Widgets::AssetDropdownTable<Texture2D>(&textures);
		}
		{
			GUI::ScopedID id(31);
			static std::unordered_map<std::string, Texture2D**> textures;
			GUI::Widgets::AssetDropdownTableMap<std::string, Texture2D>(textures);
		}

		{
			GUI::ScopedID id(32);
			static Scene* scene = *AssetManager::GetAssetRaw<Scene>(7839390104707381854);
			static u64 entityID = 5385659303079078612;
			GUI::Widgets::EntityDropdown(&entityID, scene);
		}
#endif // 0

#if 0
		{
			GUI::Properties::BeginProperties("##tag_property");
			
			static std::string tag = "Test";
			GUI::Properties::SingleLineTextInputProperty<64>(&tag, "Tag", "Tooltip");
			GUI::Properties::SingleLineTextInputDefferedProperty<64>(&tag, "Tag2", "Tooltip");
			GUI::Properties::MultiLineTextInputProperty<128>(&tag, "Tag3", "Tooltip");
			GUI::Properties::MultiLineTextInputDefferedProperty<128>(&tag, "Tag4", "Tooltip");
			static bool val = true;
			GUI::Properties::CheckboxProperty(&val, "Check", "Tooltip");
			GUI::Properties::ToggleProperty(&val, "Toggle", "Tooltip");
			GUI::Properties::ToggleButtonProperty(&val, "Toggle Button", "Tooltip", "ON", "OFF");
			static int radio = 0;
			GUI::Properties::RadioButtonProperty(&radio, {
				{ "Radio1", 0 }, { "Radio2", 1 }
			}, "Radio", "Tooltip");
			static int scalar = 0;
			GUI::Properties::ScalarInputProperty<int>(&scalar, "Scalar", "Tooltip", 1, 10, 0, 20);
			static f32 scalar2 = 0.0f;
			GUI::Properties::ScalarInputProperty<f32>(&scalar2, "Scalar2", "Tooltip", 1, 10, 0, 20);
			static f64 scalar3 = 0.0;
			GUI::Properties::ScalarInputProperty<f64>(&scalar3, "Scalar3", "Tooltip", 1, 10, 0, 20);

			static int slider = 0;
			GUI::Properties::ScalarSliderProperty<int>(&slider, "Slider", "Tooltip", 1, 10);
			static f32 slider2 = 0;
			GUI::Properties::ScalarSliderProperty<f32>(&slider2, "Slider2", "Tooltip", 1, 10);
			static f64 slider3 = 0;
			GUI::Properties::ScalarSliderProperty<f64>(&slider3, "Slider3", "Tooltip", 1, 10);

			static int drag = 0;
			GUI::Properties::ScalarDragProperty<int>(&drag, "Drag", "Tooltip", 2.f, 1, 100);
			static f32 drag2 = 0;
			GUI::Properties::ScalarDragProperty<f32>(&drag2, "Drag2", "Tooltip", 2.f, 1, 100);
			static f64 drag3 = 0;	
			GUI::Properties::ScalarDragProperty<f64>(&drag3, "Drag3", "Tooltip", 2.f, 1, 100);

			static glm::vec2 vec2 = { 0, 0 };
			GUI::Properties::Vector2InputProperty(&vec2, "Vector2", "Tooltip", 1.f, -30.f, 30.f);
			static glm::vec3 vec3 = { 0, 0, 0 };
			GUI::Properties::Vector3InputProperty(&vec3, "Vector3", "Tooltip", 1.f, -30.f, 30.f);
			static glm::vec3 vec3Color = { 0, 0, 0 };
			GUI::Properties::Vector3ColorPickerProperty(&vec3Color, "Vector3 Color", "Tooltip");
			static glm::vec4 vec4Color = { 0, 0, 0, 0 };
			GUI::Properties::Vector4ColorPickerProperty(&vec4Color, "Vector4 Color", "Tooltip");

			static int selectable = 0;
			GUI::Properties::SelectableProperty(&selectable, {
				{ "Radio1", 0 }, { "Radio2", 1 }, { "Radisdfsdfo3", 2 }, { "Radio4", 3 }, { "Radio5", 4 }
			}, "Selectable", "Tooltip");

			static AssetHandle handle = 0;
			GUI::Properties::AssetSearchProperty<Texture2D>(&handle, "Texture", "Tooltip");

			static Scene* scene = *AssetManager::GetAssetRaw<Scene>(7839390104707381854);
			static u64 entityID = 5385659303079078612;
			GUI::Properties::EntityDropdownProperty(&entityID, scene, "Entity", "Tooltip");

			static std::vector<Texture2D**> textures;
			GUI::Properties::AssetDropdownTableProperty<Texture2D>(&textures, "Texture Table", "Tooltip");

			static std::unordered_map<std::string, Texture2D**> texturesMap;
			GUI::Properties::AssetDropdownTableMapProperty<std::string, Texture2D>(&texturesMap, "Texture Table Map", "Tooltip");

			static std::filesystem::path folder;
			GUI::Properties::DrawFolderPickerProperty(&folder, ProjectContext::Get()->GetAssetDirectory(), "Folder", "Tooltip");

			static std::filesystem::path file;
			GUI::Properties::DrawFilePickerProperty(&file, ProjectContext::Get()->GetAssetDirectory(), { { "Log file", "log" } }, "File", "Tooltip");

			static std::vector<glm::vec2> vec2Table = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };
			GUI::Properties::Vector2TableProperty(&vec2Table, "Vector2 Table", "Tooltip", 1.f);

			static std::vector<glm::vec3> vec3Table = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
			GUI::Properties::Vector3TableProperty(&vec3Table, "Vector3 Table", "Tooltip", 1.f);

			GUI::Properties::EndProperties();
		}
#endif
		//ImGui::End();

		ImGui::ShowDemoWindow();
	}

}
