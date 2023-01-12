#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec2 vVertexTexture;

uniform mat3 uModelMatrix;

void main() {
    vVertexTexture = aVertexTexture;
    
  vec2 transformed = (vec3(aVertexPosition, 1)*uModelMatrix).xy;
  gl_Position = vec4(transformed, 0, 1);
    //gl_Position = vec4(aVertexPosition, 0, 1);// z=0 et homogene w=1
    
}
