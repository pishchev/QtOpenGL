#version 330

out highp vec4 fragColor;

uniform vec4 ambientColor;

uniform vec4[4] pointColor;
uniform vec4[4] pointPos;

uniform float ka;
uniform float kd;
uniform float ks;
uniform float k;
uniform int n;
uniform int p;

uniform vec4 camPos;

uniform sampler2D textureMap;
uniform sampler2D normalMap;

in vec4 normals;
in vec4 fragPos;
in vec2 texCoords;

vec3 calcNormalFromNormalmap(vec3 localNormal,vec2 uv)
{
    vec3 n = normalize(localNormal.xyz);
    vec3 t = normalize(cross(n,vec3(1,1,1)));
    vec3 b = cross(n,t);

    vec3 normal = texture(normalMap , uv).rgb;
    normal = normalize(normal*2.0f - 1.0f);

    vec3 resultingNormal = normalize(normal.x*t+normal.y*b+normal.z*n);
    return resultingNormal;
}

void main()
{
    //vec4 actualNormal  = vec4(calcNormalFromNormalmap(normals.xyz, texCoords),1);
    vec4 actualNormal = normals;

    //ambient
    vec4 col =ka* ambientColor;

    //--------------- Point -----------------//

    for (int i =0 ; i < 4 ; i++)
    {
        vec4 lightDir = normalize(pointPos[i] -fragPos);

        //specular point
        col += ks* 1/(pow(distance(fragPos , pointPos[i]),p) + k)*
                pow(max(dot( normalize(2*actualNormal - lightDir),normalize(camPos - fragPos)),0),n)*
                pointColor[i]*
                max(dot(actualNormal, lightDir),0);

        //diffuse point
        col += kd* 1/(pow(distance(fragPos , pointPos[i]),p) + k)*
                max(dot(lightDir,actualNormal),0)*
                pointColor[i];
    }


    //texture2D(texture, texCoords);
    fragColor = texture(textureMap, texCoords) * col;
}
