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
#include <iostream>
//

SDL_Window* displayWindow;
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
	
	ShaderProgram program(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");

	Matrix projectionMatrix;

	Matrix modelMatrix;
	Matrix paddle1Matrix;
	Matrix ballMatrix;

	Matrix viewModel;
	
	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	//projectionMatrix.setOrthoProjection(-4, 4, -3, 3, -1.0f, 1.0f);
	glUseProgram(program.programID);

	float lastFrameTicks = 0.0f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float angle = 0;
	int paddle1 = 0;
	int paddle2 = 0;
	const float paddleSpeed = 2;
	float disp=0;
	const Uint8 *keys;
	int ballDirection = 45;
	const float ballSpeed = .5;
	bool gameEnd = false;
	bool won;
	//
	SDL_Event event;
	bool done = false;
	while (!done) {

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			keys = SDL_GetKeyboardState(NULL);
			if (keys[SDL_SCANCODE_DOWN]) {
				if (paddle1Matrix.m[3][1] >= -1.1) {
					disp += -1 * paddleSpeed*elapsed;
					paddle1Matrix.Translate(0.0f, (float)(-1 * paddleSpeed*elapsed), 0.0f);
				}
			} else if (keys[SDL_SCANCODE_UP]) {
				if (paddle1Matrix.m[3][1] <= 1.1){
					disp += 1* paddleSpeed*elapsed;
					paddle1Matrix.Translate(0.0f, (float)(1 * paddleSpeed*elapsed), 0.0f);
				}
			}
		}
		//int cH =(float) 1.9f-.5;//ceiling collision box height
		//
		int vD = sin(ballDirection * 3.14 / 180)*ballSpeed*elapsed; //vertical Distance
		int hD = cos(ballDirection * 3.14 / 180)*ballSpeed*elapsed; //horizontal Distance
		/*
		if (ballMatrix.m[3][1] < 1.80f && vD + ballMatrix.m[3][1] >= 1.80f) {// overstep
			ballMatrix.setPosition(1.80, 0, 0);
		}
		*/
		if (!gameEnd) {
			if (ballMatrix.m[3][1] >= 1.80) {//collision ceiling
				ballDirection == 45 ? ballDirection = 315 : ballDirection = 225;
			}
			else if (ballMatrix.m[3][0] >= 3.35 && (abs(ballMatrix.m[3][1]-paddle1Matrix.m[3][1]) <= .8)) {//||(paddle1Matrix.m[3][0] ballMatrix.m[3][0])){//collision paddle 1
				ballDirection == 45 ? ballDirection = 135 : ballDirection = 225;
			}
			else if (ballMatrix.m[3][1] <= -1.80) {// && (ballDirection >= 180 && ballDirection <= 360)) {//collision floor
				ballDirection == 225 ? ballDirection = 135 : ballDirection = 45;
			}
			else if (ballMatrix.m[3][0] <= -3.35 && (abs(ballMatrix.m[3][1]-paddle1Matrix.m[3][1]) <= .8)){// && (ballDirection >= 0 && ballDirection <= 180)) {//collision paddle 2
				ballDirection == 135 ? ballDirection = 45 : ballDirection = 315;
			}
			ballMatrix.Translate(cos(ballDirection* 3.14 / 180)*ballSpeed*elapsed, sin(ballDirection * 3.14 / 180)*ballSpeed*elapsed, 0);
			if (ballMatrix.m[3][0] < -3.5){  
				gameEnd = true;
				won = true;
			}
			else if (ballMatrix.m[3][0] > 3.5) {
				gameEnd = true;
				won = false;
			}
		}
		else {
			ballMatrix.Translate(cos(ballDirection* 3.14 / 180)*ballSpeed*elapsed, sin(ballDirection * 3.14 / 180)*ballSpeed*elapsed, 0);
		}
		
		
		//ballDirection = 305;
		std::vector<float> vertices;
		
		//Ball
		vertices.push_back(-0.1f);
		vertices.push_back(-0.1f);
		
		vertices.push_back(-0.1f);
		vertices.push_back(0.1f);

		vertices.push_back(0.1f);
		vertices.push_back(0.1f);
		
		vertices.push_back(-0.1f);
		vertices.push_back(-0.1f);

		vertices.push_back(0.1f);
		vertices.push_back(-0.1f);

		vertices.push_back(0.1f);
		vertices.push_back(0.1f);

		//Ceiling
		vertices.push_back(3.55f);
		vertices.push_back(2.0f);

		vertices.push_back(3.55f);
		vertices.push_back(1.9f);

		vertices.push_back(-3.55f);
		vertices.push_back(2.0f);

		vertices.push_back(-3.55f);

		vertices.push_back(1.9f);

		vertices.push_back(3.55f);
		vertices.push_back(1.9f);

		vertices.push_back(-3.55f);
		vertices.push_back(2.0f);

		//Floor
		vertices.push_back(3.55f);
		vertices.push_back(-2.0f);

		vertices.push_back(3.55f);
		vertices.push_back(-1.9f);

		vertices.push_back(-3.55f);
		vertices.push_back(-2.0f);

		vertices.push_back(-3.55f);
		vertices.push_back(-1.9f);

		vertices.push_back(3.55f);
		vertices.push_back(-1.9f);

		vertices.push_back(-3.55f);
		vertices.push_back(-2.0f);
		
		//Paddle2
		paddle2 = vertices.size() / 2;
		vertices.push_back(-3.55);
		vertices.push_back(0.8f);

		vertices.push_back(-3.55);
		vertices.push_back(-0.8f);

		vertices.push_back(-3.45);
		vertices.push_back(0.8f);

		vertices.push_back(-3.45);
		vertices.push_back(-0.8f);

		vertices.push_back(-3.55);
		vertices.push_back(-0.8f);

		vertices.push_back(-3.45);
		vertices.push_back(0.8f);
		
		//Paddle1
		paddle1 = vertices.size() / 2;
		vertices.push_back(3.55);
		vertices.push_back(0.8f);

		vertices.push_back(3.55);
		vertices.push_back(-0.8f);

		vertices.push_back(3.45);
		vertices.push_back(0.8f);

		vertices.push_back(3.45);
		vertices.push_back(-0.8f);

		vertices.push_back(3.55);
		vertices.push_back(-0.8f);

		vertices.push_back(3.45);
		vertices.push_back(0.8f);
		//std::vector<float> vertices = { -0.5f,-0.5f, 0.5f,-0.5f, 0.5f,0.5f, -0.5f,-0.5f, 0.5f,0.5f, -0.5f,0.5f,
		//					    		1.5f,-0.5f, 2.5f,-0.5f, 2.5f,0.5f,  1.5f,-0.5f, 2.5f,0.5f,  1.5f,0.5f,
		//						       -2.5f,-0.5f, -1.5f,-0.5f, -1.5f,0.5f, -2.5f,-0.5f, -1.5f,0.5f, -2.5f,0.5f };
		//float vertices[] = { -0.5f,-0.5f, 0.5f,-0.5f, 0.5f,0.5f, -0.5f,-0.5f, 0.5f,0.5f, -0.5f,0.5f,
		//					  1.5f,-0.5f, 2.5f,-0.5f, 2.5f,0.5f,  1.5f,-0.5f, 2.5f,0.5f,  1.5f,0.5f,
		//					 -2.5f,-0.5f, -1.5f,-0.5f, -1.5f,0.5f, -2.5f,-0.5f, -1.5f,0.5f, -2.5f,0.5f };
		//


		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
		glEnableVertexAttribArray(program.positionAttribute);

		//float texCoords[] = { 0.0,1.0 ,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,
		//	0.0,1.0 ,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,
		//	0.0,1.0 ,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0 };

	    //glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		//glEnableVertexAttribArray(program.texCoordAttribute);

		//modelMatrix.identity();
		program.setModelMatrix(ballMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		program.setModelMatrix(modelMatrix);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewModel);
		//glDrawArrays(GL_TRIANGLES, paddle1, 6);
		glDrawArrays(GL_TRIANGLES, 6, (vertices.size()/2)-18);
		
		
		program.setModelMatrix(paddle1Matrix);
		glDrawArrays(GL_TRIANGLES, paddle2, 12);
		/*
		angle += elapsed;

		modelMatrix.setRotation(angle * -85 * 3.14159265359 / 180);
		program.setModelMatrix(modelMatrix);
		
		glDrawArrays(GL_TRIANGLES, 12, 6);


		modelMatrix.setRotation(angle * 110 * 3.14159265359 / 180);
		program.setModelMatrix(modelMatrix);
		
		glDrawArrays(GL_TRIANGLES, 6, 6);

		modelMatrix.identity();
		program.setModelMatrix(modelMatrix);

		
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
		*/
		//
		SDL_GL_SwapWindow(displayWindow);
		
		
		if(gameEnd){
			if(won){
				glClearColor(0, 0, 1, 0);//Blue
			}
			else{
				glClearColor(1, 0, 0, 0);//Red
			}
		}else{
			glClearColor(0, 0, 0, 0);//Added
		}
		
		glClear(GL_COLOR_BUFFER_BIT);

	}

	SDL_Quit();
	return 0;
}