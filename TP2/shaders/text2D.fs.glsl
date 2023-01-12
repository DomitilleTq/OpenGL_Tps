#version 330

in vec2 vVertexTexture;	// Pas la texture juste les u et v

out vec3 fColor;

uniform vec3 uColor;

void main() {
	fColor=uColor;
}
