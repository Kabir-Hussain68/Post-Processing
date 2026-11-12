#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform float sigma;
uniform float k;
uniform float tao;
uniform float epsilon;
uniform float phi;
uniform float colorAmount;

uniform sampler2D imageTexture;

float gaussianFunc(float dst2, float sig)
{
    return exp(-(dst2)/(2.0 * sig * sig));
}

vec3 gaussianBlur(float sig, float k)
{
    float sigma1 = sig;
    float sigma2 = sig * k;

    int radius = int(min(ceil(3.0 * sigma2), 32.0f));

    vec2 texel = 1.0 / vec2(textureSize(imageTexture, 0));

    vec3 sum1 = vec3(0.0);
    vec3 sum2 = vec3(0.0);

    float weightSum1 = 0.0f;
    float weightSum2 = 0.0f;

    for (int x = -radius; x <= radius; x++)
    {
        for (int y = -radius; y <= radius; y++)
        {
            vec2 offset = vec2(x, y);
            float dst = dot(offset, offset);

            float g1 = gaussianFunc(dst, sigma1);
            float g2 = gaussianFunc(dst, sigma2);

            vec3 l = texture(imageTexture, texCoords + offset * texel).rgb;

            sum1 += l * g1;
            weightSum1 += g1;

            sum2 += l * g2;
            weightSum2 += g2;
        }   
    }

    vec3 blur1 = sum1 / weightSum1;
    vec3 blur2 = sum2 / weightSum2;

    blur1 = blur1 * (1.0f + tao);
    blur2 = blur2 * tao;

    return blur1 - blur2;
}

void main()
{
    vec3 dog = gaussianBlur(sigma, k);
    float d = dot(dog, vec3(0.299, 0.587, 0.114));

    float edge;
    if (d >= epsilon)
    {
        edge = 1.0f;
    }
    else
    {
        edge = 1.0 + tanh(phi * (d - epsilon));
    } 

    vec3 originalColor = texture(imageTexture, texCoords).rgb;
    vec3 colorImage = originalColor * edge;

    vec3 greyScale = vec3(edge);

    fragColor = vec4(mix(greyScale, colorImage, colorAmount), 1.0f);
}