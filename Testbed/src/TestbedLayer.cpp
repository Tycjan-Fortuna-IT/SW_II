#include "TestbedLayer.hpp"

#include "Core/Renderer/RendererAPI.hpp"

namespace SW {

	void TestbedLayer::OnAttach()
	{
		GUI::Appearance::ApplyStyle(GUI::Style());
		GUI::Appearance::ApplyColors(GUI::Colors());

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
		ImGui::Begin("Testbed");

		ImGui::End();
	}

}
