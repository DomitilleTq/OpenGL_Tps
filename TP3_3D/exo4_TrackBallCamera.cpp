#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Image.hpp>
#include <glimac/TrackballCamera.hpp>
#include <cstddef>
#include <glm/glm.hpp>
//#include <glimac/common.hpp>
//#include <glimac/Geometry.hpp>
#include <glimac/Sphere.hpp>

using namespace glimac;

int main(int argc, char** argv) {

    // Initialize SDL and open a window
    GLuint largeur = 800;
    GLuint hauteur = 600;
    SDLWindowManager windowManager(largeur, hauteur, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    //---------------------------------
    // Spheres
    //---------------------------------
    
    Sphere sphere = Sphere(1, 32, 16);    
    GLuint nvertices = sphere.getVertexCount();
    const ShapeVertex* vertices = sphere.getDataPointer();


    //---------------------------------
    // Load les shaders
    //---------------------------------

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/normals.fs.glsl");
    program.use();

    //---------------------------------
    // Buffers et Vertices
    //---------------------------------
    
	GLuint vbos[3];
	glGenBuffers(3, vbos);
    
	// Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
	glBindBuffer(GL_ARRAY_BUFFER, *vbos);
	glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(ShapeVertex), vertices, GL_STATIC_DRAW);
	// Debind
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //---------------------------------
	// VAO
    //---------------------------------

	GLuint vaos[3];
	glGenVertexArrays(3, vaos);
	glBindVertexArray(*vaos);
	
	// Faire attention que le vao soit bien bind?? et que que ce soit le bon vao
	const GLuint VERTEX_ATTR_POSITION = 0;
	const GLuint VERTEX_ATTR_NORMAL = 1;
	const GLuint VERTEX_ATTR_TEXTURE = 2;
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
	
	glBindBuffer(GL_ARRAY_BUFFER, *vbos);
	glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex), offsetof(ShapeVertex, position));
	glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),  (const GLvoid*)(offsetof(ShapeVertex, normal)));
	glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,sizeof(glm::vec2),  (const GLvoid*)0);
	
    // Debind vbo et vao
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
    

    //---------------------------------
    // Variables uniformes
    //---------------------------------

    GLuint locationMVPMatrix = glGetUniformLocation(program.getGLId(),"uMVPMatrix");
    GLuint locationMVMatrix = glGetUniformLocation(program.getGLId(),"uMVMatrix");
    GLuint locationNormalMatrix = glGetUniformLocation(program.getGLId(),"uNormalMatrix");
    glEnable(GL_DEPTH_TEST);

    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), (GLfloat)largeur/(GLfloat)hauteur, 0.1f, 100.f); 
    glm::mat4 MVMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -5.f));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));


    //---------------------------------
    // Positions des lunes
    //---------------------------------

    std::vector<glm::vec3> lunePosition;
	int nlunes=32;	
	for(int i=0;i<=nlunes;i++){
		lunePosition.push_back(glm::sphericalRand(2.f));
	}
	
	//---------------------------------
    // Creation TrackBall
    //---------------------------------
	
	 TrackballCamera TrackBall = TrackballCamera();
	
	 

	
    //---------------------------------
    // Boucle des drawings
    //---------------------------------
 
    // Application loop:
    bool done = false;
    glm::ivec2 lastMousePos;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(*vaos);

        // bindez la texture sur la cible GL_TEXTURE_2D
        // glBindTexture(GL_TEXTURE_2D,textures[0]);
        
        
        // TrackBall
        
		  //TrackBall.rotateUp(float degrees)
		  //
		  bool ClickDroit = windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT);
		  glm::ivec2 Souris = windowManager.getMousePosition();
		  
		  if(ClickDroit){
		  	int dx = Souris.x - lastMousePos.x;
		  	int dy = Souris.y - lastMousePos.y;
		  	
		  	TrackBall.rotateLeft(dx);
		  	TrackBall.rotateUp(dy);
		  }
        //std::cout << Souris << std::endl;
        //TrackBall.moveFront(float delta)
		  
		  lastMousePos = Souris;
		  
		  //ProjMatrix = ProjMatrix*MatView;
        
        glm::mat4 MatView = TrackBall.getViewMatrix();
        
        glUniformMatrix4fv(locationMVMatrix,1,GL_FALSE, glm::value_ptr(MVMatrix*MatView));
        glUniformMatrix4fv(locationNormalMatrix,1,GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniformMatrix4fv(locationMVPMatrix,1,GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix*MatView));
        glDrawArrays(GL_TRIANGLES, 0, nvertices); 

        for(glm::vec3 pos : lunePosition) {
            glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5))*MatView; // Translation
            MVMatrix = glm::rotate(MVMatrix, windowManager.getTime(), glm::vec3(0, 1, 0)); // Translation * Rotation
            MVMatrix = glm::translate(MVMatrix, pos); // Translation * Rotation * Translation
            MVMatrix = glm::scale(MVMatrix, glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
            glUniformMatrix4fv(locationMVMatrix,1,GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(locationMVPMatrix,1,GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glDrawArrays(GL_TRIANGLES, 0, nvertices); 
        }


        // glBindTexture(GL_TEXTURE_2D,0);
        glBindVertexArray(0);


        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
