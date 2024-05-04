/**
 * @file Texture2D.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.5
 * @date 2024-04-06
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include "Asset/Asset.hpp"

namespace SW {

	/**
	 * @brief Represents the format of an image.
	 */
	enum class ImageFormat
	{
		None = 0, /**< No format specified */
		RED,
		RGB8, /**< RGB format with 8 bits per channel */
		RGBA8 /**< RGBA format with 8 bits per channel */
	};

	/**
	 * @brief Represents the specification of a 2D texture.
	 */
	struct TextureSpecification final
	{
		u32 Width = 1; /**< The width of the texture */
		u32 Height = 1; /**< The height of the texture */
		ImageFormat Format = ImageFormat::RGBA8; /**< The format of the texture */
	};

	/**
	 * @brief 2D texture class
	 */
	class Texture2D final : public Asset
    {
    public:
		/**
		 * @brief Construct a new Texture 2D
		 * 
		 * @param filepath Path to the texture file relative to project's assets folder
		 * @param flipped Whether the texture should be flipped vertically (must be for OpenGL related textures)
		 */
        Texture2D(const char* filepath, bool flipped = true);

		/**
		 * @brief Construct a new Texture 2D
		 * 
		 * @param filepath Path to the texture file relative to project's assets folder
		 * @param flipped Whether the texture should be flipped vertically (must be for OpenGL related textures)
		 */
		Texture2D(const std::filesystem::path& filepath, bool flipped = true);

		/**
		 * @brief Construct a new Texture 2D
		 *
		 * @param width Width of the texture
		 * @param height Height of the texture
		 */
		Texture2D(u32 width, u32 height);

		Texture2D(const TextureSpecification& spec);

		/**
		 * @brief Destroy the Texture 2D
		 */
        ~Texture2D();

		/**
		 * @brief Get the static type of the texture asset
		 * 
		 * @return AssetType Type of the asset
		 */
		static AssetType GetStaticType() { return AssetType::Texture2D; }
		
		/**
		 * @brief Get the type of the asset
		 * 
		 * @return AssetType Type of the asset
		 */
		AssetType GetAssetType() const override { return AssetType::Texture2D; }

		/**
		 * @brief Bind texture to the specified slot
		 * 
		 * @param slot Slot to bind texture to
		 */
        void Bind(u32 slot) const;

		/**
		 * @brief Get the OpenGL texture ID
		 *
		 * @return u32
		 */
		u32 GetTexHandle() const { return m_Handle; }

		/**
		 * @brief Get the Width of the texture
		 * 
		 * @return i32 
		 */
		i32 GetWidth() const { return m_Width; }

		/**
		 * @brief Get the Height of the texture
		 * 
		 * @return i32 
		 */
		i32 GetHeight() const { return m_Height; }

		/**
		 * @brief Get the Channels of the texture
		 * 
		 * @return i32 
		 */
		i32 GetChannels() const { return m_Channels; }

		/**
		 * @brief Change the size of the texture
		 * 
		 * @param newWidth New width of the texture
		 * @param newHeight New height of the texture
		 */
		void ChangeSize(i32 newWidth, i32 newHeight);

		/**
		 * @brief Get the bytes of the texture
		 * @warning Returned pointer must be freed by the caller!!
		 * 
		 * @return const char* 
		 */
		const char* GetBytes() const;

		/**
		 * @brief Get only the estimated size of the texture on the GPU
		 *
		 * @return u32
		 */
		u32 GetEstimatedSize() const { return (u32)m_Width * m_Height * 4; }

		/**
		 * @brief Set the texture data
		 * @warning Data must be whole texture data, width * height * channels
		 */
		void SetData(void* data, u32 size);

		/**
		 * @brief Check if two textures are the same
		 * 
		 * @param other Texture to compare to
		 * @return bool If textures are the same
		 */
		bool operator==(const Texture2D& other) const
		{
			return m_Handle == ((Texture2D&)other).m_Handle;
		}

    private:
        u32 m_Handle;				/** @brief OpenGL texture handle */
		i32 m_Width;				/** @brief Texture width */
		i32 m_Height;				/** @brief Texture height */
		i32 m_Channels;				/** @brief Texture channels */
		u32 m_DataFormat;			/** @brief Texture data format */
		u32 m_InternalFormat;		/** @brief Texture internal format */

		/**
		 * Loads the texture data from the specified file.
		 *
		 * @param filepath The path to the texture file.
		 * @param flipped  Indicates whether the texture should be vertically flipped during loading.
		 */
		void LoadTextureData(const char* filepath, bool flipped);
	};

}
