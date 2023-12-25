#pragma once

namespace SW {

    class Texture2D final
    {
    public:
        Texture2D(const std::string& filepath);
        ~Texture2D();

        void Bind(u32 slot) const;

    private:
        u32 m_TextureHandle;
        i32 m_Width, m_Height, m_Channels;
    };

}
