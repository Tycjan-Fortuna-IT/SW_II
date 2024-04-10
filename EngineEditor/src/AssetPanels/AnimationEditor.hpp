/**
 * @file AnimationEditor.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-04-09
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset/Panels/AssetEditorPanel.hpp"

namespace SW {

	class AnimationEditor final : public AssetEditorPanel
	{
	public:
		AnimationEditor(const char* name, const char* icon);

		void OnUpdate(Timestep ts) override;

		void SetAssetHandle(AssetHandle handle) override;

		void OnOpen() override;

		void OnClose() override;

		void Render() override;

	private:

	};

}

