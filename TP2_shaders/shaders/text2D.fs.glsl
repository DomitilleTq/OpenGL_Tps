#version 330

in vec2 vVertexTexture;

out vec3 fColor;
uniform vec3 uColor;
uniform sampler2D uTexture;

void main() {
	fColor=texture(uTexture,vVertexTexture).xyz;
	
	if (fColor==vec3(0,0,0) && vVertexTexture==vec2(0,0))
		fColor=uColor;

	if (fColor==vec3(0,0,0) && vVertexTexture==vec2(0,0))
		fColor=vec3(0,0,1);
}
