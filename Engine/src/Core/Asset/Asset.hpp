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

	/**
	 * @brief The FileType enum class represents the type of a file.
	 */
	enum class FileType : u8
	{
		Unknown = 0,	///< Unknown file type.
		Texture2D,		///< Texture file type.
		Directory,		///< Directory file type.
		Scene,			///< Scene file type
		Font,			///< Font file type.
		Prefab,			///< Prefab file type.
		Script,			///< Script file type.
	};

	enum class AssetType : u8
	{
		None = 0,
		Texture2D,
		Directory,
		Spritesheet,
		Font,
		Scene,
		Prefab,
		Script,
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

		static AssetType GetStaticType() { return AssetType::None; }
		virtual AssetType GetAssetType() const { return AssetType::None; }

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
		inline T* AsRaw() const
		{
			return dynamic_cast<T*>(this);
		}

		AssetState GetCurrentState() const { return m_State; }

		bool IsValid() const { return m_State == AssetState::Loaded; }

	protected:
		void SetCurrentState(AssetState state) { m_State = state; }

	private:
		AssetHandle m_Handle = 0u;
		AssetState m_State = AssetState::None;
	};

}
