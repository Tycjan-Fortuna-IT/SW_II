#include "AnimationEditor.hpp"

#include "Asset/Animation2D.hpp"
#include "Asset/Sprite.hpp"
#include "Asset/AssetManager.hpp"
#include "Asset/AssetLoader.hpp"

namespace SW {

	AnimationEditor::AnimationEditor(const char* name, const char* icon)
		: AssetEditorPanel(name, icon)
	{

	}

	void AnimationEditor::OnUpdate(Timestep ts)
	{
		m_CurrentTime += ts;
		m_FramesCount = (*m_Animation)->Sprites.size();

		if (!m_FramesCount)
			return;

		m_CurrentFrame = (int)(m_CurrentTime * (*m_Animation)->Speed) % m_FramesCount;

		if (m_CurrentFrame >= m_FramesCount) {
			m_CurrentFrame = 0;
			m_CurrentTime = 0;
		}
	}

	void AnimationEditor::SetAssetHandle(AssetHandle handle)
	{
		m_Animation = AssetManager::GetAssetRaw<Animation2D>(handle);
	}

	void AnimationEditor::OnOpen()
	{
		m_FramesCount = 0;
	}

	void AnimationEditor::OnClose()
	{

	}

	void AnimationEditor::Render()
	{
		constexpr ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_NoPadOuterX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_BordersInnerH
			| ImGuiTableFlags_ScrollY;

		if (ImGui::BeginTable("AnimationEditor_MainViewTable", 2, flags, ImGui::GetContentRegionAvail())) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			if (ImGui::Button(SW_ICON_SAVE "Save")) {
				const AssetMetaData& metadata = AssetManager::GetAssetMetaData((*m_Animation)->GetHandle());

				AssetLoader::Serialize(metadata);
			}

			GUI::BeginProperties("##animation_editor_properties");

			if (GUI::DrawFloatingPointProperty((*m_Animation)->Speed, "Speed", nullptr, -25.f, 25.f, 0.5f)) {
				m_CurrentFrame = 0;
			}
			GUI::DrawBooleanProperty((*m_Animation)->ReverseAlongX, "Flip X");
			GUI::DrawBooleanProperty((*m_Animation)->ReverseAlongY, "Flip Y");

			GUI::DrawAssetTableDropdownList<Sprite>((*m_Animation)->Sprites, "Sprites");

			GUI::EndProperties();

			ImGui::TableNextColumn();

			GUI::MoveMousePosX(20.f);
			GUI::MoveMousePosY(10.f);

			// Display animation
			if (m_FramesCount) {
				Sprite** current = (*m_Animation)->Sprites[m_CurrentFrame];
				const Texture2D* texture = (*current)->GetTexture();

				ImVec2 uv0 = {
					(*m_Animation)->ReverseAlongX ? (*current)->TexCordUpRight.x : (*current)->TexCordLeftDown.x,
					(*m_Animation)->ReverseAlongY ? (*current)->TexCordUpRight.y : (*current)->TexCordLeftDown.y
				};

				ImVec2 uv1 = {
					(*m_Animation)->ReverseAlongX ? (*current)->TexCordLeftDown.x : (*current)->TexCordUpRight.x,
					(*m_Animation)->ReverseAlongY ? (*current)->TexCordLeftDown.y : (*current)->TexCordUpRight.y
				};

				const f32 sprWidth = std::roundf(abs((*current)->TexCordUpRight.x - (*current)->TexCordLeftDown.x) * texture->GetWidth());
				const f32 sprHeight = std::roundf(abs((*current)->TexCordUpRight.y - (*current)->TexCordLeftDown.y) * texture->GetHeight());

				const f32 aspectRatio = sprWidth / sprHeight;
				constexpr f32 thumbnailSize = 256.f;
				ImVec2 displaySize = { thumbnailSize, thumbnailSize };
				const f32 minSpace = 0.0f;
				const f32 maxSpace = thumbnailSize * 0.25f;

				const f32 leftSpaceX = std::clamp(abs((thumbnailSize - sprWidth) * 0.25f), minSpace, maxSpace);
				const f32 leftSpaceY = std::clamp(abs((thumbnailSize - sprHeight) * 0.25f), minSpace, maxSpace);

				if (sprWidth != sprHeight) {
					if (aspectRatio > 1.0f) { // Image is wider than it is tall
						displaySize = { thumbnailSize, thumbnailSize / aspectRatio };
						GUI::MoveMousePosY(leftSpaceY);
					} else { // Image is taller than it is wide
						displaySize = { thumbnailSize * aspectRatio, thumbnailSize };
						GUI::MoveMousePosX(leftSpaceX);
					}
				}

				ImGui::Image(GUI::GetTextureID((*current)->GetTexture()), displaySize, uv0, uv1);
			}

			ImGui::EndTable();
		}
	}

}
