#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <cstddef>
#include <glm/glm.hpp>

using namespace glimac;

glm::mat3 translate(float tx, float ty){
	glm::mat3 M = glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(tx, ty, 1));
   return M;
}

glm::mat3 scale(float sx, float sy){
	glm::mat3 M = glm::mat3(glm::vec3(sx, 0, 0), glm::vec3(0, sy, 0), glm::vec3(0, 0, 1));
	return M;
}

glm::mat3 rotate(float a){
	glm::mat3 M = glm::mat3(glm::vec3(cos(glm::radians(a)), sin(glm::radians(a)), 0), glm::vec3(-sin(glm::radians(a)), cos(glm::radians(a)), 0), glm::vec3(0, 0, 1));
	//mat3 M = mat3(vec3(glm::cos(glm::radians(a)), glm::sin(glm::radians(a)), 0), vec3(-glm::sin(glm::radians(a)), glm::cos(glm::radians(a)), 0), vec3(0, 0, 1));
	return M;
}

struct Vertex2DUV{
	glm::vec2 position;
	glm::vec2 texture;
	
	Vertex2DUV(){}
	Vertex2DUV(glm::vec2 position_,glm::vec2 texture_)
   {
   	position = position_;
      texture = texture_;}
};
//float uTime =45;
int main(int argc, char** argv) {
	float uTime =45;
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;
    
    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/text2D.vs.glsl", applicationPath.dirPath() + "shaders/text2D.fs.glsl");
    program.use();
    GLuint valuePosition = glGetUniformLocation(program.getGLId(),"uModelMatrix");
    GLuint valuePosition2 = glGetUniformLocation(program.getGLId(),"uModelTrans");
    
    
	GLuint vbos[3];
	glGenBuffers(3, vbos);
	// A partir de ce point, le tableau vbos contient 16 identifiants de VBO
	
	// Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
	glBindBuffer(GL_ARRAY_BUFFER, *vbos);

    // => Tableau de sommets : un seul exemplaire de chaque sommet
    Vertex2DUV vertices[] = {
        Vertex2DUV(glm::vec2(-1, -1), glm::vec2(0, 0)), // Sommet 0
        Vertex2DUV(glm::vec2(1, -1), glm::vec2(0, 0)), // Sommet 1
        Vertex2DUV(glm::vec2(0, 1), glm::vec2(0, 0)), // Sommet 2
    };
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//VAO
	GLuint vaos[3];
	glGenVertexArrays(3, vaos);
	glBindVertexArray(*vaos);
	
	// Faire attention que le vao soit bien bindé et que que ce soit le bon vao
	const GLuint VERTEX_ATTR_POSITION = 0;
	const GLuint VERTEX_ATTR_TEXTURE = 1;
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
	
	glBindBuffer(GL_ARRAY_BUFFER, *vbos);
	glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE,sizeof(Vertex2DUV), offsetof(Vertex2DUV, position));
	glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 3, GL_FLOAT, GL_FALSE,sizeof(Vertex2DUV),  (const GLvoid*)(offsetof(Vertex2DUV, texture)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(*vaos);
	glBindVertexArray(0);
	

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
         
         	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(*vaos);
	uTime++;
	glm::mat3 Rotate =scale(0.5,0.5);//*translate(0.5,1);//rotate(uTime)*
	glm::mat3 Trans = translate(1,0.5);
	
	glUniformMatrix3fv (valuePosition,1,GL_FALSE ,glm::value_ptr(Rotate));
	glUniformMatrix3fv (valuePosition2,1,GL_FALSE ,glm::value_ptr(Trans));
	glDrawArrays(GL_TRIANGLES, 0, 3); // Si on veut dessiner plus de triangle le dernier paramétre serait plus grand
	glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }
	    glDeleteBuffers(3, vbos);
    glDeleteVertexArrays(3,vaos);
    return EXIT_SUCCESS;
}
