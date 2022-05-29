// Версия шейдера 3.3
#version 330

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

in vec4 LightSpacePos;
in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

out vec4 FragColor;

struct BaseLight {
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight {
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation {
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight {
    BaseLight Base;
    vec3 Position;
    Attenuation Attenuation;
};

struct SpotLight {
    PointLight Base;
    vec3 Direction;
    float Cutoff;
};

uniform DirectionalLight gDirectionalLight;

uniform int gNumPointLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];

uniform int gNumSpotLights;
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];

uniform sampler2D gSampler;

uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gMatSpecularPower;

uniform sampler2D gShadowMap;

float calcShadowFactor(vec4 LightSpacePos) {
    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;

    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;

    float z= 0.5 * ProjCoords.z + 0.5;

    float Depth = texture(gShadowMap, UVCoords).x;
    if (Depth < (z + 0.00001))
    return 0.5;
    else
    return 1.0;
}

vec4 calcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal, float ShadowFactor) {
    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec4 DiffuseColor = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(Light.Color, 1.0f) *
        Light.DiffuseIntensity * DiffuseFactor;

        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        SpecularFactor = pow(SpecularFactor, gMatSpecularPower);
        if (SpecularFactor > 0) {
            SpecularColor = vec4(Light.Color, 1.0f)
            * gMatSpecularIntensity * SpecularFactor;
        }
    }

    return (AmbientColor + ShadowFactor + DiffuseColor + SpecularColor);
}

vec4 calcDirectionalLight(vec3 Normal) {
    return calcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal, 1.0);
}

vec4 calcPointLight(PointLight l, vec3 Normal, vec4 LightSpacePos) {
    vec3 LightDirection = WorldPos0 - l.Position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);
    float ShadowFactor = calcShadowFactor(LightSpacePos);

    vec4 Color = calcLightInternal(l.Base, LightDirection, Normal, ShadowFactor);
    float Attenuation = l.Attenuation.Constant +
    l.Attenuation.Linear * Distance +
    l.Attenuation.Exp * Distance * Distance;

    return Color / Attenuation;
}

vec4 calcSpotLight(SpotLight l, vec3 Normal, vec4 LightSpacePos) {
    vec3 LightToPixel = normalize(WorldPos0 - l.Base.Position);
    float SpotFactor = dot(LightToPixel, l.Direction);

    if (SpotFactor > l.Cutoff) {
        vec4 Color = calcPointLight(l.Base, Normal, LightSpacePos);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));
    } else {
        return vec4(0, 0, 0, 0);
    }
}

void main() {
    vec3 Normal = normalize(Normal0);
    vec4 TotalLight = calcDirectionalLight(Normal);

    for (int i = 0; i < gNumPointLights; i++) {
        TotalLight += calcPointLight(gPointLights[i], Normal, LightSpacePos);
    }

    for (int i = 0; i < gNumSpotLights; i++) {
        TotalLight += calcSpotLight(gSpotLights[i], Normal, LightSpacePos);
    }

    FragColor = texture2D(gSampler, TexCoord0.xy) * TotalLight;
}

