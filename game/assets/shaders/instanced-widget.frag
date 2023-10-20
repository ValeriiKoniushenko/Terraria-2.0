#version 460 core

out vec4 FragColor;

in vec2 ioCv;
in vec2 ioTextRectSize;

uniform sampler2D uTexture;
uniform float uGamma;
uniform float uBrightness;
uniform float uContrast;
uniform float uSaturation;
uniform vec4 uBorderColor;
uniform float uBorderWidth;
uniform bool uIsDrawBorder;
uniform bool uHasTexture;

mat4 brightnessMatrix(float brightness)
{
    return mat4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    brightness, brightness, brightness, 1);
}

mat4 contrastMatrix(float contrast)
{
    float t = (1.0 - contrast) / 2.0;

    return mat4(
    contrast, 0, 0, 0,
    0, contrast, 0, 0,
    0, 0, contrast, 0,
    t, t, t, 1);

}

mat4 saturationMatrix(float saturation)
{
    vec3 luminance = vec3(0.3086, 0.6094, 0.0820);

    float oneMinusSat = 1.0 - saturation;

    vec3 red = vec3(luminance.x * oneMinusSat);
    red+= vec3(saturation, 0, 0);

    vec3 green = vec3(luminance.y * oneMinusSat);
    green += vec3(0, saturation, 0);

    vec3 blue = vec3(luminance.z * oneMinusSat);
    blue += vec3(0, 0, saturation);

    return mat4(
    red, 0,
    green, 0,
    blue, 0,
    0, 0, 0, 1);
}

bool canDrawBorder()
{
    return (ioCv.x <= uBorderWidth || ioCv.y <= uBorderWidth || 1.f - ioCv.x <= uBorderWidth || 1.f - ioCv.y <= uBorderWidth);
}

void main()
{
    if (uIsDrawBorder && canDrawBorder())
    {
        FragColor = uBorderColor;
    }
    else
    {
        vec2 cv = ioCv + vec2(ioTextRectSize);
        vec4 textureColor = texture(uTexture, cv);
        vec3 diffuseColor = pow(texture(uTexture, cv).rgb, vec3(uGamma));
        vec4 midColor = vec4(diffuseColor, textureColor.a);
        vec4 outColor = brightnessMatrix(uBrightness) * contrastMatrix(uContrast) * saturationMatrix(uSaturation) * midColor;

        if (!uHasTexture)
        {
            outColor.a = 0.f;
        }

        FragColor = outColor;
    }
}