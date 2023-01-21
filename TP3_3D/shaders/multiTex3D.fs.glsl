#version 330

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec3 fColor;

uniform mat4 uNormalMatrix;
uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

void main() {
	fColor=texture(uEarthTexture,vTexCoords).xyz
		   +texture(uCloudTexture,vTexCoords).xyz;
}
