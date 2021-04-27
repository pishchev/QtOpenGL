#version 330

in vec4 posAttr;
in vec4 norAttr;
in vec2 texCoord;

uniform mat4 viewmatrix;
uniform mat4 trmatrix;
uniform mat4 rtmatrix;

out vec4 normals;
out vec4 fragPos;
out vec2 texCoords;

void main()
{
    texCoords = texCoord;

    normals = normalize(transpose(inverse(rtmatrix)) * norAttr);
    fragPos = trmatrix*rtmatrix*posAttr;

    gl_Position = viewmatrix* trmatrix*rtmatrix * posAttr;
}
