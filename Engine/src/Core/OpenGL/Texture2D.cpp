#include "Texture2D.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Core/Project/ProjectContext.hpp"
#include "Core/Project/Project.hpp"

namespace SW {

    Texture2D::Texture2D(const char* filepath, bool flipped /*= true*/)
	{
		m_Path = filepath;

		LoadTextureData(filepath, flipped);
	}

	Texture2D::Texture2D(u32 width, u32 height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		m_Width = width;
		m_Height = height;
		m_Channels = 0;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
		glTextureStorage2D(m_Handle, 1, GL_RGBA8, m_Width, m_Height);

		glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture2D::Texture2D(const std::filesystem::path& filepath, bool flipped /*= true*/)
	{
		std::filesystem::path texturePath = ProjectContext::Get()->GetAssetDirectory() / filepath;
		m_Path = filepath.string();

		LoadTextureData(texturePath.string().c_str(), flipped);
	}

	Texture2D::Texture2D(const TextureSpecification& spec)
	{
		switch (spec.Format) {
			case ImageFormat::RGB8: {
				m_DataFormat = GL_RGB; m_InternalFormat = GL_RGB8; break;
			}
			case ImageFormat::RGBA8: {
				m_DataFormat = GL_RGBA; m_InternalFormat = GL_RGBA8; break;
			}
		}

		m_Width = spec.Width;
		m_Height = spec.Height;
		m_Channels = 0;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
		glTextureStorage2D(m_Handle, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_Handle);

		SW_INFO("Texture2D `{}` unloaded successfully!", m_Path);
	}

	void Texture2D::Bind(u32 slot) const
	{
		glBindTextureUnit(slot, m_Handle);
    }

	void Texture2D::SetData(void* data, u32 size)
	{
		u32 bpp = m_DataFormat == GL_RGBA ? 4 : 3;

		ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");

		glTextureSubImage2D(m_Handle, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::LoadTextureData(const char* filepath, bool flipped)
	{
		stbi_set_flip_vertically_on_load(flipped);
		stbi_uc* data = stbi_load(filepath, &m_Width, &m_Height, &m_Channels, 0);

		std::string filePath = filepath;
		std::string error = "Failed to load the image: ";
		ASSERT(data, (filePath + error).c_str());

		GLenum internalFormat = 0, dataFormat = 0;

		if (m_Channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (m_Channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else {
			SW_ERROR("Texture2D format not yet supported!");
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
		glTextureStorage2D(m_Handle, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_Handle, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(m_Handle);

		stbi_image_free(data);

		m_DataFormat = dataFormat;
		m_InternalFormat = internalFormat;

		SW_INFO("Texture2D `{}` created successfully!", filepath);
	}

}
