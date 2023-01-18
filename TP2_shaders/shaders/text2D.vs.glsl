#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec2 vVertexTexture;
uniform float uTime;

mat3 translate(float tx, float ty){
	mat3 M = mat3(vec3(1, 0, 0), vec3(0, 1, 0), vec3(tx, ty, 1));
  return M;
}

mat3 scale(float sx, float sy){
	mat3 M = mat3(vec3(sx, 0, 0), vec3(0, sy, 0), vec3(0, 0, 1));
	return M;
}

mat3 rotate(float a){
	mat3 M = mat3(vec3(cos(radians(a)), sin(radians(a)), 0), vec3(-sin(radians(a)), cos(radians(a)), 0), vec3(0, 0, 1));
	return M;
}

void main() {
    vVertexTexture = aVertexTexture;
    
    mat3 modelMatrix = rotate(uTime);
    vec2 transformed = (vec3(aVertexPosition, 1)*modelMatrix).xy;
    gl_Position = vec4(transformed, 0, 1);
    //gl_Position = vec4(aVertexPosition, 0, 1);// z=0 et homogene w=1
    
}
