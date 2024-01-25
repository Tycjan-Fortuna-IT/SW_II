/**
 * @file Texture2D.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.3
 * @date 2024-01-25
 *
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

namespace SW {

	/**
	 * @brief 2D texture class
	 */
    class Texture2D final
    {
    public:
		/**
		 * @brief Construct a new Texture 2D
		 * 
		 * @param filepath Path to the texture file relative to assets folder
		 * @param flipped Whether the texture should be flipped vertically (must be for OpenGL related textures)
		 */
        Texture2D(const std::string& filepath, bool flipped = true);

		/**
		 * @brief Construct a new Texture 2D
		 *
		 * @param width Width of the texture
		 * @param height Height of the texture
		 */
		Texture2D(u32 width, u32 height);

		/**
		 * @brief Destroy the Texture 2D
		 */
        ~Texture2D();

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
		u32 GetHandle() const { return m_Handle; }

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
		 * @brief Get only the estimated size of the texture on the GPU
		 *
		 * @return u32
		 */
		u32 GetEstimatedSize() const { return m_Width * m_Height * 4; }

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

		/**
		 * @brief Get the path to the original texture file from which the texture was loaded.
		 *
		 * @return std::string
		 */
		const std::string& GetPath() const { return m_Path; }

    private:
        u32 m_Handle;				/** @brief OpenGL texture handle */
		i32 m_Width;				/** @brief Texture width */
		i32 m_Height;				/** @brief Texture height */
		i32 m_Channels;				/** @brief Texture channels */
		u32 m_DataFormat;			/** @brief Texture data format */
		u32 m_InternalFormat;		/** @brief Texture internal format */
		std::string m_Path = "";	/** @brief Path to the texture file */
	};

}
