#version 330

in vec4 posAttr;
in vec4 norAttr;
in vec2 texCoord;

uniform mat4 viewmatrix;
uniform mat4 trmatrix;
uniform mat4 rtmatrix;
uniform mat4 lightSpaceMatrix;

out vec4 normals;
out vec4 fragPos;
out vec2 texCoords;
out vec4 fragPosLightSpace;

void main()
{
    texCoords = texCoord;

    normals = normalize(transpose(inverse(rtmatrix)) * norAttr);
    fragPos = trmatrix*rtmatrix*posAttr;

    fragPosLightSpace = lightSpaceMatrix*fragPos;

    gl_Position = viewmatrix* fragPos;
}
