#include "Texture2D.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace SW {

    Texture2D::Texture2D(const std::string& filepath) {
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_Channels, 0);

        ASSERT(data, ("Failed to load the image: " + filepath).c_str());

        GLenum internalFormat = 0, dataFormat = 0;

        if (m_Channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } else if (m_Channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        } else {
            ERROR("Texture2D format not yet supported!");
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureHandle);
        glTextureStorage2D(m_TextureHandle, 1, internalFormat, m_Width, m_Height);

        glTextureParameteri(m_TextureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_TextureHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_TextureHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_TextureHandle, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_TextureHandle);

        stbi_image_free(data);

        TRACE("Texture2D `%s` created successfully!", filepath.c_str());
    }

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &m_TextureHandle);
    }

    void Texture2D::Bind(u32 slot) const {
        glBindTextureUnit(slot, m_TextureHandle);
    }

}
