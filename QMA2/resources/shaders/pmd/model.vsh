/* pmd/model.vsh */
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewInverseMatrix;
uniform mat4 lightViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform vec3 materialAmbient;
uniform vec4 materialDiffuse;
uniform bool isMainSphereMap;
uniform bool isSubSphereMap;
uniform bool hasDepthTexture;
attribute vec4 inPosition;
attribute vec3 inNormal;
attribute vec2 inTexCoord;
attribute vec2 inToonTexCoord;
varying vec4 outColor;
varying vec4 outTexCoord;
varying vec4 outShadowCoord;
varying vec2 outToonTexCoord;
const float kTwo = 2.0;
const float kOne = 1.0;
const float kHalf = 0.5;
const vec3 kOne3 = vec3(kOne, kOne, kOne);

vec2 makeSphereMap(const vec3 position, const vec3 normal) {
    vec3 R = reflect(position, normal);
    float M = kTwo * sqrt(R.x * R.x + R.y * R.y + (R.z + kOne) * (R.z + kOne));
    return vec2(R.x / M + kHalf, R.y / M + kHalf);
}

void main() {
    vec3 view = normalize(inPosition.xyz);
    vec4 position = modelViewProjectionMatrix * inPosition;
    vec2 mainTexCoord = isMainSphereMap ? makeSphereMap(view, inNormal) : inTexCoord;
    vec2 subTexCoord = isSubSphereMap ? makeSphereMap(view, inNormal) : inTexCoord;
    outColor.rgb = min(materialAmbient + lightColor * materialDiffuse.rgb, kOne3);
    outColor.a = materialDiffuse.a;
    outTexCoord = vec4(mainTexCoord, subTexCoord);
    outToonTexCoord = vec2(0, 1.0 - dot(normalize(lightDirection), inNormal) * 0.5);
    if (hasDepthTexture) {
        outShadowCoord = lightViewProjectionMatrix * inPosition;
    }
    gl_Position = position;
}

