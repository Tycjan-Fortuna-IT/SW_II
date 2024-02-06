#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

in vec2 v_TexCoord;
in vec4 v_Color;
in flat float v_TexIndex;
in float v_TilingFactor;
in flat int v_EntityID;

uniform sampler2D u_Textures[32];

void main()
{
    int textureIndex = int(v_TexIndex);

    o_Color = texture(u_Textures[textureIndex], v_TexCoord * v_TilingFactor) * v_Color;

    o_EntityID = v_EntityID;
}
