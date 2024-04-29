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

	template <int N = 64>
	static bool SingleLineTextInputProperty(
		std::string* text, const char* label, const char* toooltip = nullptr, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None
	) {
		GUI2::Properties::BeginPropertyGrid(label, toooltip);

		bool modified = GUI2::Components::SingleLineTextInput<N>(text, flags);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	template <int N = 64>
	static bool SingleLineTextInputDefferedProperty(
		std::string* text, const char* label, const char* tooltip = nullptr, ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::SingleLineTextInput<N>(text, flags);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	template <int N = 128>
	static bool MultiLineTextInputProperty(
		std::string* text, const char* label, const char* tooltip = nullptr, const ImVec2& size = ImVec2(0, 0),
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_None
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::MultiLineTextInput<N>(text, size, flags);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	template <int N = 128>
	static bool MultiLineTextInputDefferedProperty(
		std::string* text, const char* label, const char* tooltip = nullptr, const ImVec2& size = ImVec2(0, 0),
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::MultiLineTextInput<N>(text, size, flags);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	static bool CheckboxProperty(bool* value, const char* label, const char* tooltip = nullptr, bool center = true)
	{
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::Checkbox(value, center);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	static bool ToggleProperty(bool* value, const char* label, const char* tooltip = nullptr, bool center = true)
	{
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::Toggle(value, center);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	static bool ToggleButtonProperty(
		bool* value, const char* label, const char* tooltip = nullptr, const char* whenOnLabel = nullptr,
		const char* whenOffLabel = nullptr, bool center = true, ImGuiButtonFlags buttonFlags = ImGuiButtonFlags_None
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::ToggleButton(value, whenOnLabel, whenOffLabel, center, buttonFlags);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	template <typename T>
	static bool RadioButtonProperty(
		T* value, const std::vector<GUI2::Components::SelectOption<T>>& options, const char* label, const char* tooltip = nullptr,
		bool center = true, ImGuiComboFlags flags = ImGuiComboFlags_None
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::RadioButton(value, options, center, flags);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	template <typename T>
		requires std::is_scalar_v<T>
	static bool ScalarInputProperty(
		T* value, const char* label, const char* tooltip = nullptr, T step = (T)1, T fastStep = (T)10,
		const char* format = nullptr, ImGuiInputFlags flags = ImGuiInputTextFlags_None
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::ScalarInput(value, step, fastStep, format, flags);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	template <typename T>
		requires std::is_scalar_v<T>
	static bool ScalarSliderProperty(
		T* value, const char* label, const char* tooltip = nullptr, T min = (T)1, T max = (T)10,
		const char* format = nullptr, ImGuiInputFlags flags = ImGuiInputTextFlags_None
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::ScalarSlider(value, min, max, format, flags);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	template <typename T>
		requires std::is_scalar_v<T>
	static bool ScalarDragProperty(
		T* value, const char* label, const char* tooltip = nullptr, float speed = 1.f, T min = (T)1, T max = (T)10,
		const char* format = nullptr, ImGuiInputFlags flags = ImGuiInputTextFlags_None
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::ScalarDrag(value, speed, min, max, format, flags);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	static bool Vector2InputProperty(
		glm::vec2* vector, const char* label, const char* tooltip = nullptr, f32 resetValue = 0.f,
		f32 min = -FLT_MAX, f32 max = FLT_MAX, const std::string& format = "%.2f"
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::Vector2Input(vector, resetValue, min, max, format);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	static bool Vector3InputProperty(
		glm::vec3* vector, const char* label, const char* tooltip = nullptr, f32 resetValue = 0.f,
		f32 min = -FLT_MAX, f32 max = FLT_MAX, const std::string& format = "%.2f"
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::Vector3Input(vector, resetValue, min, max, format);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	static bool Vector3ColorPickerProperty(
		glm::vec3* vector, const char* label, const char* tooltip = nullptr, ImGuiColorEditFlags flags = ImGuiColorEditFlags_None
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::Vector3ColorPicker(vector, flags);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	static bool Vector4ColorPickerProperty(
		glm::vec4* vector, const char* label, const char* tooltip = nullptr,
		ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::Vector4ColorPicker(vector, flags);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	template <typename T>
	static bool SelectableProperty(
		T* value, const std::vector<GUI2::Components::SelectOption<T>>& options, const char* label, const char* tooltip = nullptr,
		ImGuiComboFlags flags = ImGuiComboFlags_None
	) {
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Components::Selectable(value, options, flags);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	template <typename T>
		requires std::is_base_of_v<Asset, T>
	static bool AssetSearchProperty(AssetHandle* handle, const char* label, const char* tooltip = nullptr)
	{
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Widgets::AssetSearch<T>(handle);

		GUI2::Properties::EndPropertyGrid();

		return modified;
	}

	static bool EntityDropdownProperty(u64* ID, Scene* scene, const char* label, const char* tooltip = nullptr)
	{
		GUI2::Properties::BeginPropertyGrid(label, tooltip);

		bool modified = GUI2::Widgets::EntityDropdown(ID, scene);

		GUI2::Properties::EndPropertyGrid();

		return modified;
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
			GUI2::Widgets::DrawFolderPicker(&val, ProjectContext::Get()->GetAssetDirectory());
		}

		ImGui::NewLine();


		{
			GUI2::ScopedID id(27);
			static std::filesystem::path val;
			GUI2::Widgets::DrawFilePicker(&val, ProjectContext::Get()->GetAssetDirectory(), {{ "Log file", "log" }});
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
		{
			GUI2::ScopedID id(29);
			static AssetHandle handle = 0;
			GUI2::Widgets::AssetSearch<Texture2D>(&handle);
		}

		{
			GUI2::ScopedID id(30);
			static std::vector<Texture2D**> textures;
			GUI2::Widgets::AssetDropdownTable<Texture2D>(&textures);
		}
		{
			GUI2::ScopedID id(31);
			static std::unordered_map<std::string, Texture2D**> textures;
			GUI2::Widgets::AssetDropdownTableMap<std::string, Texture2D>(textures);
		}

		{
			GUI2::ScopedID id(32);
			static Scene* scene = *AssetManager::GetAssetRaw<Scene>(7839390104707381854);
			static u64 entityID = 5385659303079078612;
			GUI2::Widgets::EntityDropdown(&entityID, scene);
		}
#endif // 0

		{
			GUI2::Properties::BeginProperties("##tag_property");
			
			static std::string tag = "Test";
			SingleLineTextInputProperty<64>(&tag, "Tag", "Tooltip");
			SingleLineTextInputDefferedProperty<64>(&tag, "Tag2", "Tooltip");
			MultiLineTextInputProperty<128>(&tag, "Tag3", "Tooltip");
			MultiLineTextInputDefferedProperty<128>(&tag, "Tag4", "Tooltip");
			static bool val = true;
			CheckboxProperty(&val, "Check", "Tooltip");
			ToggleProperty(&val, "Toggle", "Tooltip");
			ToggleButtonProperty(&val, "Toggle Button", "Tooltip", "ON", "OFF");
			static int radio = 0;
			RadioButtonProperty(&radio, {
				{ "Radio1", 0 }, { "Radio2", 1 }
			}, "Radio", "Tooltip");
			static int scalar = 0;
			ScalarInputProperty<int>(&scalar, "Scalar", "Tooltip", 1, 10);
			static f32 scalar2 = 0.0f;
			ScalarInputProperty<f32>(&scalar2, "Scalar2", "Tooltip", 1, 10);
			static f64 scalar3 = 0.0;
			ScalarInputProperty<f64>(&scalar3, "Scalar3", "Tooltip", 1, 10);

			static int slider = 0;
			ScalarSliderProperty<int>(&slider, "Slider", "Tooltip", 1, 10);
			static f32 slider2 = 0;
			ScalarSliderProperty<f32>(&slider2, "Slider2", "Tooltip", 1, 10);
			static f64 slider3 = 0;
			ScalarSliderProperty<f64>(&slider3, "Slider3", "Tooltip", 1, 10);

			static int drag = 0;
			ScalarDragProperty<int>(&drag, "Drag", "Tooltip", 2.f, 1, 100);
			static f32 drag2 = 0;
			ScalarDragProperty<f32>(&drag2, "Drag2", "Tooltip", 2.f, 1, 100);
			static f64 drag3 = 0;	
			ScalarDragProperty<f64>(&drag3, "Drag3", "Tooltip", 2.f, 1, 100);

			static glm::vec2 vec2 = { 0, 0 };
			Vector2InputProperty(&vec2, "Vector2", "Tooltip", 1.f, -30.f, 30.f);
			static glm::vec3 vec3 = { 0, 0, 0 };
			Vector3InputProperty(&vec3, "Vector3", "Tooltip", 1.f, -30.f, 30.f);
			static glm::vec3 vec3Color = { 0, 0, 0 };
			Vector3ColorPickerProperty(&vec3Color, "Vector3 Color", "Tooltip");
			static glm::vec4 vec4Color = { 0, 0, 0, 0 };
			Vector4ColorPickerProperty(&vec4Color, "Vector4 Color", "Tooltip");

			static int selectable = 0;
			SelectableProperty(&selectable, {
				{ "Radio1", 0 }, { "Radio2", 1 }, { "Radisdfsdfo3", 2 }, { "Radio4", 3 }, { "Radio5", 4 }
			}, "Selectable", "Tooltip");

			static AssetHandle handle = 0;
			AssetSearchProperty<Texture2D>(&handle, "Texture", "Tooltip");

			static Scene* scene = *AssetManager::GetAssetRaw<Scene>(7839390104707381854);
			static u64 entityID = 5385659303079078612;
			EntityDropdownProperty(&entityID, scene, "Entity", "Tooltip");

			GUI2::Properties::EndProperties();
		}
		

		ImGui::End();

		ImGui::ShowDemoWindow();
	}

}
