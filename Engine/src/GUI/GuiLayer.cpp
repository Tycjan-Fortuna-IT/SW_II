#include "GuiLayer.hpp"

#include <imgui.h>
#include "GUI/ImGuiBuild.hpp"

#include "Core/Application.hpp"

namespace SW {

	GuiLayer::GuiLayer()
		: Layer("GUI Layer") {}

	void GuiLayer::OnAttach() {
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

		ImGui_ImplGlfw_InitForOpenGL(Application::Get()->GetWindow()->GetHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void GuiLayer::OnDetach() {
		
	}

	void GuiLayer::OnUpdate(float dt) {
		
	}

	void GuiLayer::OnRender() {
		
	}

	void GuiLayer::Begin() const {
		
	}

	void GuiLayer::End() const {
		
	}

}
