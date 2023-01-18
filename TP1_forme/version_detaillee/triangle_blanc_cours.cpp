#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>

using namespace glimac;

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);
    
    // Initialisation de GLEW; nous verrons dans le prochain TP à quoi cela sert.
    GLint error;
    if(GLEW_OK != (error = glewInit())) {
        std::cerr << "Impossible d'initialiser Glew" << std::endl;
        return 1;
    }
    
    // Création d'un Vertex Buffer Object et d'un Vertex Array Object
    GLuint vbo, vao;
    
    // Allocation d'un Vertex Buffer Object:
    glGenBuffers(1, &vbo);

    // "vbo" devient le VBO courant sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
        // Tableau des attributs des sommets
        GLfloat vertices[] = {
            -0.5, -0.5, // Premier vertex
            0.5, -0.5, // Deuxième vertex
            0., 0.5, // Troisème vertex
        };
        
        // Stockage des données du tableau vertices dans le vbo placé sur GL_ARRAY_BUFFER (c'est à dire "vbo" ici) :
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Plus de VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Allocation d'un Vertex Array Objet:
    glGenVertexArrays(1, &vao);
    
    // "vao" devient le VAO courant:
    glBindVertexArray(vao);
    
        // "vbo" devient le VBO courant sur la cible GL_ARRAY_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        
        // Activation de l'attribut de vertex 0; nous l'interpretons comme la position
        glEnableVertexAttribArray(0 /* Incide attribut*/);
        // On spécifie le type de donnée de l'attribut position ainsi que la manière dont il est stocké dans le VBO
        glVertexAttribPointer(
            0 /* Indice attribut */, 
            2 /* Nombre de composantes */, 
            GL_FLOAT /* Type d'une composante */, 
            GL_FALSE /* Pas de normalisation */, 
            2 * sizeof(GLfloat) /* Taille en octet d'un vertex complet entre chaque attribut position */, 
            0 /* OpenGL doit utiliser le VBO attaché à GL_ARRAY_BUFFER et commencer à l'offset 0 */);
        
    // Plus de VAO courant:
    glBindVertexArray(0);
    
    bool loop = true;
    while(loop) {
        // "vao" devient le VAO courant, OpenGL l'utilisera lors du dessin pour avoir des informations sur les sommets
        glBindVertexArray(vao);
        
        // OpenGL doit dessiner des triangles en utilisant 3 sommets (donc un seul triangle)
        glDrawArrays(GL_TRIANGLES, 0 /* Pas d'offset au début du VBO */, 3);
        
        // Plus de VAO courant:
        glBindVertexArray(0);
        
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                loop = false;
            }
        }

        SDL_GL_SwapBuffers();
    }
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    
    SDL_Quit();
    return 0;
}
