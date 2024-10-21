#include "EditorSettings.hpp"

namespace SW
{

	EditorSettings& EditorSettings::Get()
	{
		static EditorSettings s_Settings;

		return s_Settings;
	}

} // namespace SW
