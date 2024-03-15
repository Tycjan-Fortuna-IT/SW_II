#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

in vec2 v_TexCoord;
in vec4 v_Color;
in flat float v_TexIndex;
in flat int v_EntityID;

uniform sampler2D u_FontAtlasTextures[32];

float screenPxRange() {
	const float pxRange = 2.0;
    
    vec2 unitRange = vec2(pxRange) / vec2(textureSize(u_FontAtlasTextures[int(v_TexIndex)], 0));

    vec2 screenTexSize = vec2(1.0) / fwidth(v_TexCoord);
    
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    int textureIndex = int(v_TexIndex);

    vec3 msd = texture(u_FontAtlasTextures[textureIndex], v_TexCoord).rgb;

    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

    if (opacity == 0.0)
		discard;

    o_Color = mix(vec4(0.0), v_Color, opacity);

    o_EntityID = v_EntityID;
}
