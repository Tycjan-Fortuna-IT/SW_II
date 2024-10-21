#pragma once

#include "AssetRegistry.hpp"

namespace SW
{

	class Asset;

	// When adding new serializer class remember to register it in the AssetLoader

	class AssetSerializer
	{
	public:
		virtual ~AssetSerializer() = default;

		virtual void Serialize(const AssetMetaData& metadata) = 0;

		virtual Asset* TryLoadAsset(const AssetMetaData& metadata) = 0;
	};

	class SceneAssetSerializer final : public AssetSerializer
	{
	public:
		void Serialize(const AssetMetaData& metadata) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

	class Texture2DSerializer final : public AssetSerializer
	{
	public:
		void Serialize(const AssetMetaData& metadata) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

	class SpriteSerializer final : public AssetSerializer
	{
	public:
		void Serialize(const AssetMetaData& metadata) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

	class SpritesheetSerializer final : public AssetSerializer
	{
	public:
		void Serialize(const AssetMetaData& metadata) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

	class FontSerializer final : public AssetSerializer
	{
	public:
		void Serialize(const AssetMetaData& metadata) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

	class FontSourceSerializer final : public AssetSerializer
	{
	public:
		void Serialize(const AssetMetaData& metadata) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

	class AnimationSerializer final : public AssetSerializer
	{
	public:
		void Serialize(const AssetMetaData& metadata) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

	class SoundSerializer final : public AssetSerializer
	{
	public:
		void Serialize(const AssetMetaData& metadata) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

	class PrefabSerializer final : public AssetSerializer
	{
	public:
		void Serialize(const AssetMetaData& metadata) override;

		Asset* TryLoadAsset(const AssetMetaData& metadata) override;
	};

} // namespace SW
