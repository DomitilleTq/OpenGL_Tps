#version 330 core

in vec3 vFragColor;
in vec2 vVertexPosition;

out vec3 fFragColor;
out vec2 fVertexPosition;

void main() {
float alpha=2;
	float B=15;
  float a=alpha*exp(-B*length(vVertexPosition)*length(vVertexPosition));
  fFragColor = vFragColor*a;
  //fFragColor = vFragColor*length(fract(5.0 * vVertexPosition)); //carré
  //fFragColor = vFragColor*length(abs(fract(5.0 * vVertexPosition) * 2.0 - 1.0));
  //fFragColor = vFragColor*mod(floor(10.0 * vVertexPosition.x) + floor(10.0 * vVertexPosition.y), 2.0);
  //fFragColor = vFragColor*smoothstep(0.3, 0.32, length(fract(5.0 * vVertexPosition) - 0.5));
  //fFragColor = vFragColor*smoothstep(0.4, 0.5, max(abs(fract(8.0 * vVertexPosition.x - 0.5 * mod(floor(8.0 * vVertexPosition.y), 2.0)) - 0.5), abs(fract(8.0 * vVertexPosition.y) - 0.5)));
  //fFragColor = vFragColor*smoothstep(0.4, 0.5, max(abs(fract(8.0 * vVertexPosition.x - 0.5 * mod(floor(8.0 * vVertexPosition.y), 2.0)) - 0.5), abs(fract(8.0 * vVertexPosition.y) - 0.5)));
  fVertexPosition = vVertexPosition;
};
