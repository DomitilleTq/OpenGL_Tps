#version 330

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec3 fFragColor;

uniform float uKd; // coefficient de reflection diffuse de l'objet
uniform float uKs; // coefficient de reflection glossy de l'objet
uniform float uShininess; // exposant de brillance permettant de controller la taille de la "tache" de brillance glossy

uniform vec3 uLightDir_vs;  // direction incidente 
uniform float uLightIntensity; // intensite de la lumière incidente 

vec3 blinnPhlong(vec3 wi, vec3 Li, vec3 N, float Kd, float Ks, float shininess, vec3 halfVec){
    return Li*(Kd*dot(wi,N)+Ks*pow(dot(halfVec,N),shininess));
}

void main() {
	//fColor=vec3(0,0,1);
    
    vec3 w0 = normalize(-vPosition_vs);
    vec3 wi = normalize(uLightPos_vs - vPosition_vs);
    float d = distance(uLightPos_vs, vPosition_vs);

    fFragColor = blinnPhlong(
        wi,
        wi*uLightIntensity / (d * d),
        vNormal_vs, 
        uKd, uKs,
        uShininess,
        (w0+wi)/2
    );

    if (fFragColor==vec3(0,0,0))// && vTexCoords==vec2(0,0))
        fFragColor = vNormal_vs;
}