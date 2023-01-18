#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>

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

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // Création d'un seul VBO:
    GLuint vbo,vao;
    glGenBuffers(1, &vbo);
    // A partir de ce point, la variable vbo contient l'identifiant d'un VBO

    // Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER

    // creer un tableau de GLfloat contenant toutes ces coordonnées à la suite:
    GLfloat vertices[] = { -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f };

    // L'envoi des données :
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // débinder le vbo de la cible pour éviter de le remodifier par erreur
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // création d'un VAO est similaire à celle d'un VBO. 
    glGenVertexArrays(1,&vao);
    //binder le VAO
    glBindVertexArray(vao);

    // "vbo" devient le VBO courant sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // Par défaut, l'attribut position est identifié par 0
    const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    // indiquer à OpenGL où il va trouver les sommets à dessiner, 
    // et comment lire les informations associé à chaque sommet
    const GLint size = 2;
    const GLenum type = GL_FLOAT;
    const GLboolean normalized = GL_FALSE;
    const GLsizei stride = 2*sizeof(GLfloat);
    glVertexAttribPointer(
        VERTEX_ATTR_POSITION, 
        size, 
        type, 
        normalized, 
        stride, 
        (GLvoid*)(0*sizeof(GLfloat)));

    // débinder le VAO afin de ne pas le modifier par erreur
    glBindVertexArray(0);

    // Le code de dessin est répété à chaque tour de la boucle d'application. 
    // Il faut donc d'abord nettoyer la fenêtre afin de ne pas avoir de résidu du tour précédent.
    glClear(GL_COLOR_BUFFER_BIT);

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

        // lance le pipeline sur un ensemble de sommets spécifiés par un VAO. 
        // Il faut donc commencer par re-binder le VAO.
        glBindVertexArray(vao);
        GLenum mode = GL_TRIANGLES;
        GLint first = 0; // indice du premier sommet dans le VBO
        GLsizei count = 3; // nombre de sommets à dessiner
        glDrawArrays(mode, first, count);
        // débinder le VAO afin de ne pas le modifier par erreur
        glBindVertexArray(0);


        // Update the display
        windowManager.swapBuffers();
    }

    // libérer les ressources allouées sur GPU: le VBO et le VAO
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1,&vao);

    return EXIT_SUCCESS;
}
