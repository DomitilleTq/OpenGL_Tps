#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>


namespace glimac {

class TrackballCamera {
private:
	float m_fDistance;
	float m_fAngleX;
	float m_fAngleY;
	
public:
    TrackballCamera(){
    	m_fDistance=5;
    	m_fAngleX=0;
    	m_fAngleY=0;
    }
    
    void moveFront(float delta){
    
    	m_fDistance=m_fDistance+delta;
    } //permettant d'avancer / reculer la caméra. Lorsque delta est positif la caméra doit avancer, sinon elle doit reculer.
	void rotateLeft(float degrees){
		m_fAngleX=m_fAngleX+degrees;
	} //permettant de tourner latéralement autour du centre de vision.
	void rotateUp(float degrees){
		m_fAngleY=m_fAngleY+degrees;
	}//permettant de tourner verticalement autour du centre de vision.
	
	glm::mat4 getViewMatrix() const{		
		glm::mat4 MatriceFinal = glm::translate(glm::mat4(1), glm::vec3(0, 0, -m_fDistance)); // Translation
		MatriceFinal = glm::rotate(MatriceFinal, glm::radians(m_fAngleX), glm::vec3(0, 1, 0)); // Translation * Rotation
		MatriceFinal = glm::rotate(MatriceFinal, glm::radians(m_fAngleY), glm::vec3(1, 0, 0)); // Translation * Rotation * Rotation
		return MatriceFinal;
	}
	

};

}
