#include "GuiLayer.hpp"

#include <GLFW/glfw3.h>
#include <ImGuizmo.h>

#include "Core/Application.hpp"
#include "GUI/ImGuiBuild.hpp"

namespace SW
{

	GuiLayer::GuiLayer() : Layer("GUI Layer")
	{
	}

	void GuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

		ImGui_ImplGlfw_InitForOpenGL(Application::Get()->GetWindow()->GetWindowHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void GuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GuiLayer::OnUpdate(Timestep dt)
	{
		ImGuiIO& io      = ImGui::GetIO();
		Application* app = Application::Get();
		io.DisplaySize   = ImVec2((float)app->GetWindow()->GetWidth(), (float)app->GetWindow()->GetHeight());
		io.DeltaTime     = dt;
	}

	void GuiLayer::OnRender()
	{
	}

	void GuiLayer::Begin() const
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void GuiLayer::End() const
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			glfwMakeContextCurrent(backup_current_context);
		}
	}

} // namespace SW
