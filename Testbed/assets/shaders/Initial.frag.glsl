#version 450 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Texture[32];

void main()
{
    int textureIndex = int(v_TexIndex);

    color = texture(u_Texture[textureIndex], v_TexCoord * v_TilingFactor) * v_Color;
}
