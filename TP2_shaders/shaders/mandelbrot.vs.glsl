#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vColor;
out vec2 vVertexPosition;

void main() {
    vColor = aVertexColor;
    //vColor = vec3(aVertexPosition.x,aVertexPosition.y,0);
    vVertexPosition=aVertexPosition;
    gl_Position = vec4(aVertexPosition, 0, 1);// z=0 et homogene w=1
}
