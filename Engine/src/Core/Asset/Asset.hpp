/**
 * @file Asset.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-29
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

	using AssetHandle = u64;

	enum class AssetType : u8
	{
		Unknown = 0,
		Directory,
		Texture2D,
		Sprite,
		Spritesheet,
		Font,
		Scene,
		Prefab,
		Script,
		Shader,
	};

	enum class AssetState : u8
	{
		None = 0,
		Invalid,
		Loaded,
		Loading
	};

	class Asset
	{
	public:
		Asset() = default;
		virtual ~Asset() = default;

		static AssetType GetStaticType() { return AssetType::Unknown; }
		virtual AssetType GetAssetType() const { return AssetType::Unknown; }

		virtual bool operator==(const Asset& other) const
		{
			return m_Handle == other.m_Handle;
		}

		virtual bool operator!=(const Asset& other) const
		{
			return !(*this == other);
		}

		template <typename T>
			requires std::is_base_of_v<Asset, T>
		inline const T* As() const
		{
			return dynamic_cast<const T*>(this);
		}

		template <typename T>
			requires std::is_base_of_v<Asset, T>
		inline T* AsRaw()
		{
			return dynamic_cast<T*>(this);
		}

		AssetHandle GetHandle() const { return m_Handle; }
		AssetState GetCurrentState() const { return m_State; }

		bool IsValid() const { return m_State == AssetState::Loaded; }

	protected:
		void SetCurrentState(AssetState state) { m_State = state; }

	private:
		AssetHandle m_Handle = 0u;
		AssetState m_State = AssetState::None;

		friend class EditorAssetManager; // Handle is set by the AssetManager!
	};

}
