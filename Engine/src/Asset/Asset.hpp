/**
 * @file Asset.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Thumbnail.hpp"

namespace SW {

	class Texture2D;

	using AssetHandle = u64;

	/**
	 * @brief Enum class representing the type of an asset.
	 */
	enum class AssetType : u8
	{
		Unknown = 0,
		Directory,
		Texture2D,
		Sprite,
		Spritesheet,
		Font,
		FontSource,
		Scene,
		Prefab,
		Script,
		Shader,
		Animation2D,
		AssetRegistry,
	};

	/**
	 * @brief Enum class representing the state of an asset.
	 * @warning NOT USED YET!
	 */
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

		/**
		 * @brief Get the type of the asset.
		 * 
		 * @return AssetType 
		 */
		static AssetType GetStaticType() { return AssetType::Unknown; }

		/**
		 * @brief Get the type of the asset.
		 * 
		 * @return AssetType 
		 */
		virtual AssetType GetAssetType() const { return AssetType::Unknown; }

		/**
		 * @brief Get the stringified type of the asset.
		 * 
		 * @return const char* 
		 */
		static const char* GetStringifiedAssetType(AssetType type);

		/**
		 * @brief Get the asset type from a string.
		 * 
		 * @param type The stringified type.
		 * @return AssetType 
		 */
		static AssetType GetAssetTypeFromStringified(const std::string& type);

		/**
		 * @brief Get the asset type from an extension.
		 * 
		 * @param extension The extension.
		 * @return AssetType 
		 */
		static AssetType GetAssetTypeFromExtension(const std::string& extension);

		/**
		 * @brief Get the MDI font icon of the asset type.
		 * 
		 * @param type The asset type.
		 * @return const char* 
		 */
		static const char* GetIconFromAssetType(AssetType type);

		/**
		 * @brief Get the color of the asset type.
		 * 
		 * @param type The asset type.
		 * @return u32 
		 */
		static u32 GetColorFromAssetType(AssetType type);

		/**
		 * @brief Get the thumbnail of the asset type.
		 * 
		 * @param type The asset type.
		 * @return Thumbnail 
		 */
		static Thumbnail GetThumbnailFromAssetType(AssetType type);
		
		/**
		 * @brief Equality operator.
		 * @param other The other asset to compare with.
		 * 
		 * @return bool True if the assets are equal, false otherwise.
		 */
		virtual bool operator==(const Asset& other) const
		{
			return m_Handle == other.m_Handle;
		}

		/**
		 * @brief Inequality operator.
		 * @param other The other asset to compare with.
		 * 
		 * @return bool True if the assets are not equal, false otherwise.
		 */
		virtual bool operator!=(const Asset& other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Casts the asset to the specified type.
		 * 
		 * @tparam T The type to cast to.
		 * @return const T* The casted asset.
		 */
		template <typename T>
			requires std::is_base_of_v<Asset, T>
		inline const T* As() const
		{
			return dynamic_cast<const T*>(this);
		}

		/**
		 * @brief Casts the asset to the specified type.
		 * 
		 * @tparam T The type to cast to.
		 * @return T* The casted asset.
		 */
		template <typename T>
			requires std::is_base_of_v<Asset, T>
		inline T* AsRaw()
		{
			return dynamic_cast<T*>(this);
		}

		/**
		 * @brief Get the asset handle.
		 * 
		 * @return AssetHandle 
		 */
		AssetHandle GetHandle() const { return m_Handle; }

		/**
		 * @brief Set the asset handle.
		 * @warning BE CAREFUL WITH THIS! TODO As of now a workaround for scene asset handling.
		 */
		void SetHandle(AssetHandle handle) { m_Handle = handle; }
		
		/**
		 * @brief Get the asset state.
		 * 
		 * @return AssetState 
		 */
		AssetState GetCurrentState() const { return m_State; }

		/**
		 * @brief Check if the asset is valid.
		 * 
		 * @return bool True if the asset is valid, false otherwise.
		 */
		bool IsValid() const { return m_State == AssetState::Loaded; }

	protected:
		/**
		 * @brief Set the asset handle.
		 * 
		 * @param handle The asset handle.
		 */
		void SetCurrentState(AssetState state) { m_State = state; }

	private:
		AssetHandle m_Handle = 0u;				///< The handle of the asset.
		AssetState m_State = AssetState::None;	///< The state of the asset.

		friend class EditorAssetManager; // Handle is set by the AssetManager!
	};

}
