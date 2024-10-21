#include "Texture2D.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Core/Project/ProjectContext.hpp"
#include "Core/Project/Project.hpp"

namespace SW {

	static int DataFormatToChannels(GLenum format)
	{
		switch (format) {
			case GL_RED: return 1;
			case GL_RG: return 2;
			case GL_RGB: return 3;
			case GL_RGBA: return 4;
		}

		SYSTEM_ERROR("Unknown format: {}", format);
		return 0;
	}

    Texture2D::Texture2D(const char* filepath, bool flipped /*= true*/)
	{
		LoadTextureData(filepath, flipped);
	}

	Texture2D::Texture2D(u32 width, u32 height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		m_Width = (i32)width;
		m_Height = (i32)height;
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
		std::filesystem::path texturePath = filepath;

		LoadTextureData(texturePath.string().c_str(), flipped);
	}

	Texture2D::Texture2D(const TextureSpecification& spec)
	{
		switch (spec.Format) {
			case ImageFormat::RED: {
				m_DataFormat = GL_RED; m_InternalFormat = GL_RED; break;
			}
			case ImageFormat::RGB8: {
				m_DataFormat = GL_RGB; m_InternalFormat = GL_RGB8; break;
			}
			case ImageFormat::RGBA8: {
				m_DataFormat = GL_RGBA; m_InternalFormat = GL_RGBA8; break;
			}
		}

		m_Width = (i32)spec.Width;
		m_Height = (i32)spec.Height;
		m_Channels = DataFormatToChannels(m_DataFormat);

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
	}

	void Texture2D::Bind(u32 slot) const
	{
		glBindTextureUnit(slot, m_Handle);
    }

    void Texture2D::ChangeSize(i32 newWidth, i32 newHeight)
    {
		u32 newTextureHandle;
		glCreateTextures(GL_TEXTURE_2D, 1, &newTextureHandle);
		glTextureStorage2D(newTextureHandle, 1, m_InternalFormat, newWidth, newHeight);

		glTextureParameteri(newTextureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(newTextureHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(newTextureHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(newTextureHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
		// https://stackoverflow.com/questions/30286424/what-is-texture-downsampling-downscaling-using-opengl
		GLuint fboIds[2] = { 0 };
		glGenFramebuffers(2, fboIds);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, fboIds[0]);
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							   GL_TEXTURE_2D, m_Handle, 0);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboIds[1]);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							   GL_TEXTURE_2D, newTextureHandle, 0);

		glBlitFramebuffer(0, 0, m_Width, m_Height,
						  0, 0, newWidth, newHeight,
						  GL_COLOR_BUFFER_BIT, GL_LINEAR);
		
		glDeleteTextures(1, &m_Handle);
		glDeleteFramebuffers(2, fboIds);

		m_Handle = newTextureHandle;
		m_Width = newWidth;
		m_Height = newHeight;
	}

	const char* Texture2D::GetBytes() const
	{
		GLint bpp = DataFormatToChannels(m_DataFormat);
		GLint size = m_Width * m_Height * bpp;

		char* data = new char[size];

		glBindTexture(GL_TEXTURE_2D, m_Handle);
		glGetTexImage(GL_TEXTURE_2D, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);

		return data;
	}

	void Texture2D::SetData(void* data, u32 size)
	{
		u32 bpp = DataFormatToChannels(m_DataFormat);

		ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");

		glTextureSubImage2D(m_Handle, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::LoadTextureData(const char* filepath, bool flipped)
	{
		stbi_set_flip_vertically_on_load(flipped);
		stbi_uc* data = stbi_load(filepath, &m_Width, &m_Height, &m_Channels, 0);

		ASSERT(data, "Failed to load the image: {}", filepath);

		GLenum internalFormat = 0, dataFormat = 0;

		if (m_Channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		} else if (m_Channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		} else {
			SYSTEM_ERROR("Texture2D format not yet supported!");
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

		SYSTEM_INFO("Texture2D `{}` created successfully!", filepath);
	}

}
