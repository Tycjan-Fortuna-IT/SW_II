#pragma once

namespace SW {

	class TestbedLayer final : public Layer {
	public:
		explicit TestbedLayer(const std::string& name)
			: Layer(name) {}

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep dt) override;
		void OnRender() override;
	};

}
