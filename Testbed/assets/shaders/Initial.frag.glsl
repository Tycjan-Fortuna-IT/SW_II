#version 450 core

out vec4 color;

in vec3 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{
    // color = vec4(v_Color, 1.0f);
    // color = texture(u_Texture1, v_TexCoord);
    color = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.2);
    // color = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, vec2(1.0 - v_TexCoord.x, 1.0 - v_TexCoord.y)), 0.2);
}
