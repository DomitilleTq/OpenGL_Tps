#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Image.hpp>
#include <cstddef>
#include <glm/glm.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/TrackballCamera.hpp>

using namespace glimac;

// ------------------------------------------
// Structs pour utiliser plusieurs shaders
// ------------------------------------------

struct EarthProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;

    GLint uNormalMatrix;
    // GLint uEarthTexture;
    // GLint uCloudTexture;


    GLint uKd; // coefficient de reflection diffuse de l'objet
    GLint uKs; // coefficient de reflection glossy de l'objet
    GLint uShininess; // exposant de brillance permettant de controller la taille de la "tache" de brillance glossy
    GLint uLightDir_vs;  // direction incidente 
    GLint uLightIntensity; // intensite de la lumière incidente 


    EarthProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/directionallight.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        // uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        // uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
        uKd = glGetUniformLocation(m_Program.getGLId(), "uKd");
        uKs = glGetUniformLocation(m_Program.getGLId(), "uKs");
        uShininess = glGetUniformLocation(m_Program.getGLId(), "uShininess");
        uLightDir_vs = glGetUniformLocation(m_Program.getGLId(), "uLightDir_vs");
        uLightIntensity = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity");
    }
};

struct MoonProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    // GLint uTexture;
    

    GLint uKd; // coefficient de reflection diffuse de l'objet
    GLint uKs; // coefficient de reflection glossy de l'objet
    GLint uShininess; // exposant de brillance permettant de controller la taille de la "tache" de brillance glossy
    GLint uLightDir_vs;  // direction incidente 
    GLint uLightIntensity; // intensite de la lumière incidente 


    MoonProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/directionallight.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        // uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
        uKd = glGetUniformLocation(m_Program.getGLId(), "uKd");
        uKs = glGetUniformLocation(m_Program.getGLId(), "uKs");
        uShininess = glGetUniformLocation(m_Program.getGLId(), "uShininess");
        uLightDir_vs = glGetUniformLocation(m_Program.getGLId(), "uLightDir_vs");
        uLightIntensity = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity");
    }
};


// ------------------------------------------
// Fonctions pour automatiser
// ------------------------------------------

std::unique_ptr<Image> loadAndBindTextures(std::string img_src, GLuint * textures, int position) {
    
    std::unique_ptr<Image> img_ptr = loadImage(img_src);
    if (!img_ptr) 
   	    std::cout << "imgTriforce null " << std::endl;

    // binder la texture sur la cible GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D,textures[position]);
    // envoyer img au GPU pour stockée dans la texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        img_ptr->getWidth(), img_ptr->getHeight(),
        0, GL_RGBA, GL_FLOAT,
        (const void *)img_ptr->getPixels()
    );
    // spécifier les filtres à appliquer
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // débinder la texture
    glBindTexture(GL_TEXTURE_2D,0);

    return img_ptr;
} 


// ------------------------------------------
// MAIN 
// ------------------------------------------

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

    // Les lunes autours de la terre
    std::vector<glm::vec3> lunePosition;
	int nlunes=32;	
	for(int i=0;i<=nlunes;i++){
		lunePosition.push_back(glm::sphericalRand(2.f));
	}

    //---------------------------------
    // Load les shaders 
    // => a été remplacé par le chargement des shaders (pour chacune des structures)
    //---------------------------------
    
    FilePath applicationPath(argv[0]);
    EarthProgram earthProgram(applicationPath);
    MoonProgram moonProgram(applicationPath);
    

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
	
	// Faire attention que le vao soit bien bindé et que que ce soit le bon vao
	const GLuint VERTEX_ATTR_POSITION = 0;
	const GLuint VERTEX_ATTR_NORMAL = 1;
	const GLuint VERTEX_ATTR_TEXTURE = 2;
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
	
	glBindBuffer(GL_ARRAY_BUFFER, *vbos);
	glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex), offsetof(ShapeVertex, position));
	glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),  (const GLvoid*)(offsetof(ShapeVertex, normal)));
	glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),  (const GLvoid*)(offsetof(ShapeVertex, texCoords)));
	
    // Debind vbo et vao
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
    

    //---------------------------------
    // Matrices de projection
    //---------------------------------

    // Activer le test de profondeur du GPU. 
    // Pour eviter que certains triangles non visibles recouvrent des triangles situés devant.
    glEnable(GL_DEPTH_TEST);

    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), (GLfloat)largeur/(GLfloat)hauteur, 0.1f, 100.f); 
    glm::mat4 MVMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -5.f));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

	
	//---------------------------------
    // Creation TrackBall
    //---------------------------------
	
	 TrackballCamera TrackBall = TrackballCamera();
			
    //---------------------------------
    // Boucle des drawings
    //---------------------------------
 
    GLfloat uKd = 0.5; // coefficient de reflection diffuse de l'objet
    GLfloat uKs = 0.5; // coefficient de reflection glossy de l'objet
    GLfloat uShininess = 0.5; // exposant de brillance (taille de brillance glossy)
    glm::vec3 uLightDir_vs = glm::vec3(1,1,1);  // direction incidente 
    GLfloat uLightIntensity = 1.0; // intensite de la lumière incidente 


    glm::ivec2 lastMousePos;

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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(*vaos);

        // Avant le dessin de chaque entitée (terre ou lune), 
        // changer le programme de shader utilisé en utilisant la méthode use()
        earthProgram.m_Program.use();

         // TrackBall
        
		  bool ClickDroit = windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT);
		  glm::ivec2 Souris = windowManager.getMousePosition();
		  
		  if(ClickDroit){
		  	int dx = Souris.x - lastMousePos.x;
		  	int dy = Souris.y - lastMousePos.y;
		  	
		  	TrackBall.rotateLeft(dx);
		  	TrackBall.rotateUp(dy);
		  }
		  lastMousePos = Souris;

        glm::mat4 MatView = TrackBall.getViewMatrix();
        

    glUniform1f(earthProgram.uKd,uKd);
    glUniform1f(earthProgram.uKs,uKs);
    glUniform1f(earthProgram.uShininess,uShininess);
    glUniform3fv(earthProgram.uLightDir_vs,1,glm::value_ptr(uLightDir_vs));
    glUniform1f(earthProgram.uLightIntensity,uLightIntensity);

        glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix*MatView));
        glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(NormalMatrix))));
        glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix*MatView));
        
        // Dessiner
        glDrawArrays(GL_TRIANGLES, 0, nvertices); 


        for(glm::vec3 pos : lunePosition) {
    glUniform1f(earthProgram.uKd,uKd);
    glUniform1f(earthProgram.uKs,uKs);
    glUniform1f(earthProgram.uShininess,uShininess);
    glUniform3fv(earthProgram.uLightDir_vs,1,glm::value_ptr(uLightDir_vs));
    glUniform1f(earthProgram.uLightIntensity,uLightIntensity);

            glm::mat4 MV_lune = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5))*MatView; // Translation
            MV_lune = glm::rotate(MV_lune, windowManager.getTime(), glm::vec3(0, 1, 0)); // Translation * Rotation
            MV_lune = glm::translate(MV_lune, pos); // Translation * Rotation * Translation
            MV_lune = glm::scale(MV_lune, glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
            glUniformMatrix4fv(earthProgram.uMVMatrix,1,GL_FALSE, glm::value_ptr(MV_lune));
            glUniformMatrix4fv(earthProgram.uMVPMatrix,1,GL_FALSE, glm::value_ptr(ProjMatrix * MV_lune));
            
            // Dessiner après les bind de textures
            glDrawArrays(GL_TRIANGLES, 0, nvertices); 
        }


        // glBindTexture(GL_TEXTURE_2D,0);
        glBindVertexArray(0);


        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
