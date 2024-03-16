#include "TestbedLayer.hpp"

#include "Core/Renderer/RendererAPI.hpp"
#include "gui.hpp"

namespace SW {

	void TestbedLayer::OnAttach()
	{
		const GUI::FontSpecification fontSpec("assets/fonts/Roboto/Roboto-Regular.ttf", "assets/fonts/Roboto/Roboto-Bold.ttf");

		GUI::Appearance::ApplyStyle(GUI::Style()); 
		GUI::Appearance::ApplyColors(GUI::Colors());
		GUI::Appearance::ApplyFonts(fontSpec);

		Application::Get()->GetWindow()->SetVSync(true);

		Renderer2D::Initialize();
	}

	void TestbedLayer::OnDetach()
	{
		Renderer2D::Shutdown();
	}

	void TestbedLayer::OnUpdate(Timestep dt)
	{
		RendererAPI::Clear();
	}

	void TestbedLayer::OnRender()
	{
		GUI::CreateDockspace("Main Dockspace", nullptr);

		ImGui::Begin("Panel");

		ImGui::End();
	}

}
