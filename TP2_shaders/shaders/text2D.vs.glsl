#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec2 vVertexTexture;
uniform mat3 uModelMatrix;
uniform mat3 uModelTrans;


/*
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
*/

void main() {
     
    /*
    // calculer avec uTime
    mat3 modelMatrix = rotate(uTime);
    vec2 transformed = (vec3(aVertexPosition, 1)*modelMatrix).xy;
    */

    // calculer avec matrice recu de l'appli
    vec2 transformed = (uModelTrans*vec3(aVertexPosition, 1)*uModelMatrix).xy;
    if (transformed==vec2(0,0))
      transformed = aVertexPosition;
    
    vVertexTexture = aVertexTexture;   
    gl_Position = vec4(transformed, 0, 1);
    
}
