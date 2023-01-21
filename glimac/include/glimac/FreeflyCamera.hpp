#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>


namespace glimac {

class FreeflyCamera{
private : 
	glm::vec3 m_Position; // Position de la caméra
	float m_fPhi; // Coordonnées sphériques
	float m_fTheta; // Coordonnées sphériques
	
	glm::vec3 m_FrontVector; // Vecteur F
	glm::vec3 m_LeftVector; // Vecteur L
	glm::vec3 m_UpVector; // Vecteur U
	
	void computeDirectionVectors(){
		m_FrontVector =glm::vec3(glm::cos(m_fTheta)*glm::sin(m_fPhi),glm::sin(m_fTheta),glm::cos(m_fTheta)*glm::cos(m_fPhi)); 
		m_LeftVector =glm::vec3(glm::sin(m_fPhi+(glm::pi<float>()/2)),0,glm::cos(m_fPhi+(glm::pi<float>()/2))); 
		m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
	}

public : 
	FreeflyCamera(){
		m_Position = glm::vec3(0,0,0);
    	m_fPhi=glm::pi<float>();
    	m_fTheta=0;
    	computeDirectionVectors();
    }
    
    void moveLeft(float t){
    	m_Position += t*m_LeftVector;
    	computeDirectionVectors();
    }
    
    void moveFront(float t){
    	m_Position += t*m_FrontVector;
    	computeDirectionVectors();
    }
    
    void rotateLeft(float degrees){
    	m_fTheta = m_fTheta + glm::radians(degrees);
    	computeDirectionVectors();
    }
    void rotateUp(float degrees){
    	m_fPhi = m_fPhi + glm::radians(degrees);
    	computeDirectionVectors();
    }
    
    glm::mat4 getViewMatrix() const{
    	glm::vec3 point = m_Position+m_FrontVector;
    	return glm::lookAt(m_Position, point, m_UpVector);
    }

};

}
