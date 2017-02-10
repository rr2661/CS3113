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
#include <vector>
//

class Entity {
public:
	Entity()
		:id(0), speed(0.0f), x(0), y(0)
	{
		//lastId++;
		//lastVNum += 3;
	}
	void Draw() {
		//glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, (id*6)-1, 6);
	}
	void change(ShaderProgram* program, Matrix* modelMatrix) {
		program->setModelMatrix(*modelMatrix);
		Draw();
		modelMatrix->identity();
	}
	void rotate(ShaderProgram* program, Matrix* modelMatrix, float elapsed, float angle) {
		modelMatrix->setRotation(angle * elapsed * 3.14159265359 / 180);
		change(program, modelMatrix);
	}
	//static int lastVNum;
	//static int lastId;
	int id;

	float speed;
	float directionX;
	float directionY;

	float x;
	float y;
	float rotation;

	float width;
	float height;
};


SDL_Window* displayWindow;

void setup(SDL_GLContext* context, Matrix* projectionMatrix, ShaderProgram* program) {
	SDL_GL_MakeCurrent(displayWindow, *context);
#ifdef _WINDOWS
	glewInit();
#endif
	glViewport(0, 0, 640, 360);
	projectionMatrix->setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	glUseProgram(program->programID);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void cleanup() {
	SDL_GL_SwapWindow(displayWindow);
	glClearColor(0.4f, 0.4f, 0.4f, 0.4f);//Added
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
		
	//ShaderProgram program(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	ShaderProgram program(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewModel;

	float lastFrameTicks = 0.0f;
	float angle = 0;
	SDL_Event event;
	bool done = false;

	setup(&context, &projectionMatrix, &program);

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
		
		float vertices[] = {-0.5f,-0.5f, 0.5f,-0.5f, 0.5f,0.5f, -0.5f,-0.5f, 0.5f,0.5f, -0.5f,0.5f, //ball
						     1.5f,-0.5f, 2.5f,-0.5f, 2.5f,0.5f,  1.5f,-0.5f, 2.5f,0.5f,  1.5f,0.5f, //paddle1
							-2.5f,-0.5f, -1.5f,-0.5f, -1.5f,0.5f, -2.5f,-0.5f, -1.5f,0.5f, -2.5f,0.5f }; //paddle2
		
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		
		//float texCoords[] = { 0.0,1.0 ,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,
		//					  0.0,1.0 ,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0, 
		//					  0.0,1.0 ,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0 };

		//glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		//glEnableVertexAttribArray(program.texCoordAttribute);
		
		angle += elapsed;
		
		Entity ball;
		ball.Draw();
		cleanup();
	}

	SDL_Quit();
	return 0;
}

/*
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
*/