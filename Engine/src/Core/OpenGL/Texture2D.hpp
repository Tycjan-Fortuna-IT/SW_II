/**
 * @file Texture2D.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.2
 * @date 2024-01-09
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
		u32 GetHandle() const { return m_TextureHandle; }

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

    private:
        u32 m_TextureHandle;	/** @brief OpenGL texture handle */
		i32 m_Width;			/** @brief Texture width */
		i32 m_Height;			/** @brief Texture height */
		i32 m_Channels;			/** @brief Texture channels */
    };

}
