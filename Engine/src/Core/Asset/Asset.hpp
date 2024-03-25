/**
 * @file Asset.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.0
 * @date 2024-03-17
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

	using AssetHandle = u64;

	enum class AssetType : u8
	{
		None = 0,
		Texture,
		Spritesheet,
		Font,
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
		Asset(const std::filesystem::path& path);
		virtual ~Asset() {}

		AssetHandle GetHandle() const { return m_Handle; }
		const std::filesystem::path& GetPath() { return m_Path; }

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
		T* As()
		{
			return dynamic_cast<T*>(this);
		}

		AssetState GetCurrentState() const { return m_State; }

	protected:
		void SetCurrentState(AssetState state) { m_State = state; }

	private:
		AssetHandle m_Handle = 0u;
		
		AssetState m_State = AssetState::None;

		std::filesystem::path m_Path = "";
	};

}
