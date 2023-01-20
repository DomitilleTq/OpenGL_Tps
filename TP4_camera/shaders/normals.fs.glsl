#version 330

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec3 fColor;


void main() {
	//fColor=vec3(0,0,1);
	fColor=normalize(vNormal_vs);
	//fColor=texture(uTexture,vVertexTexture).xyz;
}