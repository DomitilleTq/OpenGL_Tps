#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec2 vVertexTexture;

void main() {
    vVertexTexture = aVertexTexture;
    gl_Position = vec4(aVertexPosition, 0, 1);// z=0 et homogene w=1
    
}
