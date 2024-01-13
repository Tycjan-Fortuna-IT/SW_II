#version 450 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Texture[32];

void main()
{
    int index = int(v_TexIndex);
    color = v_Color;

    // color = vec4(v_Color, 1.0f);
    // color = texture(u_Texture1, v_TexCoord);
    // color = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.2);
    // color = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, vec2(1.0 - v_TexCoord.x, 1.0 - v_TexCoord.y)), 0.2);
}
