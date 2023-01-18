#version 330

in vec2 vVertexTexture;

out vec3 fColor;
uniform vec3 uColor;
uniform sampler2D uTexture;

void main() {
	//fColor=vec3(0,0,1);
	//fColor=uColor;
	fColor=texture(uTexture,vVertexTexture).xyz;
}
