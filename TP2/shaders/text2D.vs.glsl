#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec2 vVertexTexture;

uniform mat3 uModelMatrix;
uniform mat3 uModelTrans;

void main() {
    vVertexTexture = aVertexTexture;
    //mat3 Trans = translate(0.5,0);
    
  vec2 transformed = (uModelTrans*vec3(aVertexPosition, 1)*uModelMatrix).xy;
  gl_Position = vec4(transformed, 0, 1);
    //gl_Position = vec4(aVertexPosition, 0, 1);// z=0 et homogene w=1
    
}
