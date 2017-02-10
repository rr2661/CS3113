#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
//
#include "Matrix.h"
#include "ShaderProgram.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//

SDL_Window* displayWindow;

GLuint LoadTexture(const char *filePath) {
	int w, h, comp;
	unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);

	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}

	GLuint retTexture;
	glGenTextures(1, &retTexture);
	glBindTexture(GL_TEXTURE_2D, retTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(image);
	return retTexture;
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif
	//
		glViewport(0, 0, 640, 360);
		ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
		//ShaderProgram program(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");

		GLuint emojiTexture = LoadTexture(RESOURCE_FOLDER"emoji.png");
		GLuint laserGreenShot = LoadTexture(RESOURCE_FOLDER"laserGreenShot.png");
		GLuint laserRedShot = LoadTexture(RESOURCE_FOLDER"laserRedShot.png");

		Matrix projectionMatrix;
		Matrix modelMatrix;
		Matrix viewModel;

		projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);

		glUseProgram(program.programID);

		float lastFrameTicks = 0.0f;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		float angle = 0;
	//
	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		//

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		
		
		
		float vertices[] = {-0.5f,-0.5f, 0.5f,-0.5f, 0.5f,0.5f, -0.5f,-0.5f, 0.5f,0.5f, -0.5f,0.5f,
						     1.5f,-0.5f, 2.5f,-0.5f, 2.5f,0.5f,  1.5f,-0.5f, 2.5f,0.5f,  1.5f,0.5f,
							-2.5f,-0.5f, -1.5f,-0.5f, -1.5f,0.5f, -2.5f,-0.5f, -1.5f,0.5f, -2.5f,0.5f };
		//
		
		

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		
		float texCoords[] = { 0.0,1.0 ,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,
							  0.0,1.0 ,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0, 
							  0.0,1.0 ,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0 };

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);

		modelMatrix.identity();
		program.setModelMatrix(modelMatrix);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewModel);

		angle += elapsed;

		modelMatrix.setRotation(angle * -85 * 3.14159265359/180);
		program.setModelMatrix(modelMatrix);
		glBindTexture(GL_TEXTURE_2D, laserRedShot);
		glDrawArrays(GL_TRIANGLES, 12, 6);


		modelMatrix.setRotation(angle * 110 * 3.14159265359/180);
		program.setModelMatrix(modelMatrix);
		glBindTexture(GL_TEXTURE_2D, laserGreenShot);
		glDrawArrays(GL_TRIANGLES, 6, 6);

		modelMatrix.identity();
		program.setModelMatrix(modelMatrix);

		glBindTexture(GL_TEXTURE_2D, emojiTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//
		SDL_GL_SwapWindow(displayWindow);
		glClearColor(0.4f, 0.4f, 0.4f, 0.4f);//Added
		glClear(GL_COLOR_BUFFER_BIT);
		
	}

	SDL_Quit();
	return 0;
}
