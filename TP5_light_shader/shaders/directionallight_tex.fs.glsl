#version 330

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec3 fFragColor;

uniform mat4 uNormalMatrix;
uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

uniform float uKd; // coefficient de reflection diffuse de l'objet
uniform float uKs; // coefficient de reflection glossy de l'objet
uniform float uShininess; // exposant de brillance permettant de controller la taille de la "tache" de brillance glossy
uniform vec3 uLightDir_vs;  // direction incidente 
uniform float uLightIntensity; // intensite de la lumière incidente 

vec3 blinnPhlong(vec3 wi, vec3 Li, vec3 N, float Kd, float Ks, float shininess, vec3 halfVec){
    return Li*(Kd*dot(wi,N)+Ks*pow(dot(halfVec,N),shininess));
}

void main() {	
    
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightDir_vs);

	  fFragColor=texture(uEarthTexture,vTexCoords).xyz
		          +texture(uCloudTexture,vTexCoords).xyz;

    fFragColor *= blinnPhlong(
        wi,
        uLightIntensity*wi,
        vNormal_vs, 
        uKd, uKs,
        uShininess,
        (w0+wi)/2
    );

    //if (fFragColor==vec3(0,0,0))// && vTexCoords==vec2(0,0))
      //  fFragColor = vNormal_vs;
    
    //if (fFragColor==vec3(0,0,0))// && vTexCoords==vec2(0,0))
      //  fFragColor = vec3(1,1,1)*0.5;
}