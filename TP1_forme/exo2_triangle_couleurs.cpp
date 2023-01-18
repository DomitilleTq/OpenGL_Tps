#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

using namespace glimac;

int main(int argc, char** argv) {
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
    Program program = loadProgram(applicationPath.dirPath() + "shaders/triangle.vs.glsl", applicationPath.dirPath() + "shaders/triangle.fs.glsl");
    program.use();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
     
   // Création de plusieurs VBO
	GLuint vbos[3];
	glGenBuffers(3, vbos);
	// A partir de ce point, le tableau vbos contient 16 identifiants de VBO
	
	// Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
	glBindBuffer(GL_ARRAY_BUFFER, *vbos);
	// On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER
	
	// Remplissage du VBO
	GLfloat vertices[] = { 
        -0.5f, -0.5f, 1.f, 0.f, 0.f, 
        0.5f, -0.5f, 0.f, 1.f, 0.f, 
        0.0f, 0.5f, 0.f, 0.f, 1.f 
    };
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	
	// Débinde
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//VAO
	GLuint vaos[3];
	glGenVertexArrays(3, vaos);
	glBindVertexArray(*vaos);
	
	// Faire attention que le vao soit bien bindé et que que ce soit le bon vao
	const GLuint VERTEX_ATTR_POSITION = 3;
	const GLuint VERTEX_ATTR_COLOR = 8;
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
	
	glBindBuffer(GL_ARRAY_BUFFER, *vbos);
	glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE,5 * sizeof(GLfloat), 0);
	glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE,5 * sizeof(GLfloat),  (const GLvoid*)(2 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(*vaos);
	glBindVertexArray(0);
     
     /*********************************
     * END OF INITIALIZATION
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
	glDrawArrays(GL_TRIANGLES, 0, 3); // Si on veut dessiner plus de triangle le dernier paramétre serait plus grand
	glBindVertexArray(0);
         
         /*********************************
         * END OF THE RENDERING CODE
         *********************************/

        // Update the display
        windowManager.swapBuffers();
    }
    glDeleteBuffers(3, vbos);
    glDeleteVertexArrays(3,vaos);
    return EXIT_SUCCESS;
}
