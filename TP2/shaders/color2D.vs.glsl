#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;
out vec2 vVertexPosition;

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
	//mat3 M = mat3(vec3(glm::cos(glm::radians(a)), glm::sin(glm::radians(a)), 0), vec3(-glm::sin(glm::radians(a)), glm::cos(glm::radians(a)), 0), vec3(0, 0, 1));
	return M;
}

void main() {
	
  vFragColor = aVertexColor;
  vVertexPosition=aVertexPosition;
 
  // Translation 
  /*vec2 translation=vec2(0.5);
  gl_Position = vec4(aVertexPosition+translation, 0, 1);// translation*/
  
  /*mat3 M = translate(0.5,0.5);
  vec2 transformed = (M * vec3(aVertexPosition, 1)).xy;
  gl_Position = vec4(transformed, 0, 1);*/
  
  // Scale
  //gl_Position = vec4(aVertexPosition*2, 0, 1);// double taille triangle
  
  /*mat3 M = scale(2,2);
  vec2 transformed = (M * vec3(aVertexPosition, 1)).xy;
  gl_Position = vec4(transformed, 0, 1);*/
  
  //gl_Position = vec4(aVertexPosition.x*2,aVertexPosition.y/2, 0, 1);// double x et moitié y
  
  /*mat3 M = scale(2,0.5);
  vec2 transformed = (M * vec3(aVertexPosition, 1)).xy;
  gl_Position = vec4(transformed, 0, 1);*/
  
  // Rotation
  /*mat3 M = rotate(45);
  vec2 transformed = (M * vec3(aVertexPosition, 1)).xy;
  gl_Position = vec4(transformed, 0, 1);*/
  
  // Echainement
  /*mat3 T = translate(0.5,0);
  mat3 R = rotate(45);
  mat3 S = scale(0.5,0.5);
  vec2 transformed = (vec3(aVertexPosition, 1)*T*R*S).xy;
  gl_Position = vec4(transformed, 0, 1);*/
  
  // Halo
  gl_Position = vec4(aVertexPosition, 0, 1);
};
