#include "AssetSourceItem.hpp"

namespace SW
{

	bool AssetSourceItem::IsParentOfDescendant(const AssetSourceItem* descendant) const
	{
		if (descendant == nullptr)
			return false;

		if (descendant->Parent == this)
			return true;

		for (const auto& child : Children)
			if (child->IsParentOfDescendant(descendant))
				return true;

		return false;
	}

} // namespace SW
