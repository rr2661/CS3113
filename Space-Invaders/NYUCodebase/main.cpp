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

enum EntityType {ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_PROJECTILE};

class SheetSprite {
public:
	//SheetSprite() {
	//
	//}
	SheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY, GLuint spriteSheetTexture, float size)
		: program(program), index(index), spriteCountX(spriteCountX), spriteCountY(spriteCountY), spriteSheetTexture(spriteSheetTexture), size(size),
		spriteWidth((1.0 / (float)spriteCountX)), spriteHeight((1.0 / (float)spriteCountY)){}
	void Draw() {
		float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
		float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
		//float spriteWidth = 1.0 / (float)spriteCountX;
		//float spriteHeight = 1.0 / (float)spriteCountY;
		GLfloat texCoords[] = {
			u, v + spriteHeight,
			u + spriteWidth, v,
			u, v,
			u + spriteWidth, v,
			u, v + spriteHeight,
			u + spriteWidth, v + spriteHeight
		};
		//float vertices[] = { -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
		//	-0.5f, 0.5f, -0.5f };

		float aspect = 1;// spriteWidth / spriteHeight;
		float vertices[] = {
			-0.5f * size * aspect, -0.5f * size,
			0.5f * size * aspect, 0.5f * size,
			-0.5f * size * aspect, 0.5f * size,
			0.5f * size * aspect, 0.5f * size,
			-0.5f * size * aspect, -0.5f * size ,
			0.5f * size * aspect, -0.5f * size };

		glBindTexture(GL_TEXTURE_2D, spriteSheetTexture);
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program->positionAttribute);
		glEnableVertexAttribArray(program->texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program->positionAttribute);
		glDisableVertexAttribArray(program->texCoordAttribute);
	}
	/*
	void Draw(ShaderProgram *program) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLfloat texCoords[] = {
	u, v + height,
	u + width, v,
	u, v,
	u + width, v,
	u, v + height,
	u + width, v + height
	};
	float aspect = width / height;
	float vertices[] = {
	-0.5f * size * aspect, -0.5f * size,
	0.5f * size * aspect, 0.5f * size,
	-0.5f * size * aspect, 0.5f * size,
	0.5f * size * aspect, 0.5f * size,
	-0.5f * size * aspect, -0.5f * size ,
	0.5f * size * aspect, -0.5f * size };

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->positionAttribute);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
	}
	*/
	//void Render() {
	//	enemySprite.Draw(program);
	//}
	ShaderProgram *program;
	int index;
	int spriteCountX;
	int spriteCountY;
	GLuint spriteSheetTexture;
	float size;
	float spriteWidth;
	float spriteHeight;
};

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_image_free(image);
	return retTexture;
}
/*
void DrawSpriteSheetSprite(ShaderProgram *program, int index, int spriteCountX,	int spriteCountY, GLuint spriteSheetTexture, float size) {
float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
float spriteWidth = 1.0 / (float)spriteCountX;
float spriteHeight = 1.0 / (float)spriteCountY;
GLfloat texCoords[] = {
u, v + spriteHeight,
u + spriteWidth, v,
u, v,
u + spriteWidth, v,
u, v + spriteHeight,
u + spriteWidth, v + spriteHeight
};
//float vertices[] = { -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
//	-0.5f, 0.5f, -0.5f };

float aspect = spriteWidth / spriteHeight;
float vertices[] = {
-0.5f * size * aspect, -0.5f * size,
0.5f * size * aspect, 0.5f * size,
-0.5f * size * aspect, 0.5f * size,
0.5f * size * aspect, 0.5f * size,
-0.5f * size * aspect, -0.5f * size ,
0.5f * size * aspect, -0.5f * size };

glBindTexture(GL_TEXTURE_2D, spriteSheetTexture);
glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
glEnableVertexAttribArray(program->positionAttribute);
glEnableVertexAttribArray(program->texCoordAttribute);
glDrawArrays(GL_TRIANGLES, 0, 6);
glDisableVertexAttribArray(program->positionAttribute);
glDisableVertexAttribArray(program->texCoordAttribute);
}
*/
void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing) {
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		int spriteIndex = (int)text[i];
		float texture_x = (float)(spriteIndex % 16) / 16.0f;
		float texture_y = (float)(spriteIndex / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	// draw this data (use the .data() method of std::vector to get pointer to data)
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 2);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}
//
void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing, int ydisp, int xdisp) {
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		int spriteIndex = (int)text[i];
		float texture_x = (float)(spriteIndex % 16) / 16.0f;
		float texture_y = (float)(spriteIndex / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			(((size + spacing) * i) + (-0.5f * size)) + xdisp, (0.5f * size) + ydisp,
			(((size + spacing) * i) + (-0.5f * size)) + xdisp, (-0.5f * size) + ydisp,
			(((size + spacing) * i) + (0.5f * size)) + xdisp, (0.5f * size) + ydisp,
			(((size + spacing) * i) + (0.5f * size)) + xdisp, (-0.5f * size) + ydisp,
			(((size + spacing) * i) + (0.5f * size)) + xdisp, (0.5f * size) + ydisp,
			(((size + spacing) * i) + (-0.5f * size)) + xdisp, (-0.5f * size) + ydisp,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	// draw this data (use the .data() method of std::vector to get pointer to data)
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 2);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}


class Vector3 {
public:
	Vector3(float x, float y, float z) 
		:x(x), y(y), z(z) {
	}
	float x;
	float y;
	float z;
};

class Entity {
		enum Boundry { BOTTOM, TOP, LEFT, RIGHT};
public:
	Entity(Vector3 position, Vector3 velocity, float rotation, SheetSprite* sprite, ShaderProgram* program, EntityType type)
		:position(position), velocity(velocity), size(std::move(Vector3{ sprite->size,sprite->size,0 })), rotation(rotation),
		sprite(sprite), program(program), type(type), canCollide(type != ENTITY_PROJECTILE)
	{}
	void Updater(float elapsed) {
	//generate model matrix
		Model.identity();
		position.x += velocity.x * elapsed;
		position.y += velocity.y * elapsed;
		position.z += velocity.z * elapsed;
		Model.setPosition(position.x, position.y, position.z);
		//model.Translate(velocity.x * elapsed, velocity.y * elapsed, velocity.z * elapsed);
		//model.setScale(size.x , size.y, size.z );
	}
	void Update(float elapsed) {
		Updater(elapsed / 6);
		Updater(elapsed / 6);
		Updater(elapsed / 6);
		Updater(elapsed / 6);
		Updater(elapsed / 6);
		Updater(elapsed / 6);
	}
	void Draw() {
		/*
		if (!isDrawable) return;
		if (position.x > 4 || position.x < -4 || position.y < -3 || position.y > 3) {
			isDrawable = false;
			return;
		}
		*/
		program->setModelMatrix(Model);
		sprite->Draw();
	}
  	void Collides(Entity* collider){
		/*float myTOP = myBoundries[TOP];
		float myBOTTOM = myBoundries[BOTTOM];
		float myLEFT = myBoundries[LEFT];
		float myRIGHT = myBoundries[RIGHT];
		
		float theirTOP = theirBoundries[TOP];
		float theirBOTTOM = theirBoundries[BOTTOM];
		float theirLEFT = theirBoundries[LEFT];
		float theirRIGHT = theirBoundries[RIGHT];
		*/
		float myWidth =.09f;
		float myHeight =.125f;
		float theirWidth =.1f;
		float theirHeight =.125f;
		if(!(
			(position.y - myHeight > collider->position.y + theirHeight) ||
			(position.y + myHeight < collider->position.y - theirHeight)||
			(position.x - myHeight > collider->position.x + theirWidth) ||
			(position.x + myHeight < collider->position.x - theirWidth)
			)){
			//collision
			//exit(0);
			Reset();
			collider->Reset();
		}
		//delete myBoundries;
		//delete theirBoundries;
	}
/*
	float* getBoundries() {
		float* boundries = new float[4];
		boundries[BOTTOM] = (position.y - .5);//sprite->spriteHeight) * size.y;
		boundries[TOP] = (position.y + .5);//sprite->spriteHeight) * size.y;
		boundries[RIGHT] = (position.x + .5);//sprite->spriteWidth) * size.x;
		boundries[LEFT] = (position.x - .5);//sprite->spriteWidth) * size.x;
		float myVerRange = boundries[TOP] - .5;//boundries[BOTTOM];
		float myHorRange = boundries[RIGHT] - .5;//boundries[LEFT];
  		float dummy = boundries[BOTTOM];
		
		if ((boundries[BOTTOM]>100 || boundries[BOTTOM]<-100) ||
			(boundries[TOP]>100 || boundries[TOP]<-100) ||
			(boundries[LEFT]>100 || boundries[LEFT]<-100) ||
			(boundries[RIGHT]>100 || boundries[RIGHT]<-100) )
		{
			exit(-1);
		}
		
		
		boundries[BOTTOM]= position.y - (sprite->spriteHeight * sprite->size *.5f);
		boundries[TOP]= position.y + (sprite->spriteHeight * sprite->size *.5f);
		boundries[LEFT]= position.x - (sprite->spriteWidth * sprite->size *.5f);
		boundries[RIGHT]= position.x + (sprite->spriteWidth * sprite->size *.5f);
		
  		return boundries;
	}
*/
	void Reset() {
		position = std::move(Vector3{ 9, 9, 0 });
		velocity = std::move(Vector3{ 0, 0, 0 });
		canCollide = false;
	}
	Vector3 position;
	Vector3 velocity;
	Vector3 size;
	float rotation;
	SheetSprite* sprite;
	ShaderProgram* program;
	EntityType type;
	bool canCollide;
	Matrix Model;
};
void Render(std::vector<Entity>& entities, ShaderProgram* program, float elapsed) {
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < entities.size(); i++) {
		entities[i].Draw();
	}
}
void Update(std::vector<Entity>& entities, float elapsed) {
	for (int i = 0; i < entities.size(); i++) {
		entities[i].Update(elapsed);
	}
}
void shootBullet(std::vector<Entity>* entities, int& bulletIndex, int MAX_BULLETS) {
	(*entities)[bulletIndex].position.x = (*entities)[0].position.x;
	(*entities)[bulletIndex].position.y = (*entities)[0].position.y;
	(*entities)[bulletIndex].position.z = (*entities)[0].position.z;
	(*entities)[bulletIndex].velocity.y = 1;
	(*entities)[bulletIndex].canCollide = true;
	bulletIndex++;
	if (bulletIndex > MAX_BULLETS) {
		bulletIndex = 1;
	}
}
/*
void Update(float elapsed) {
	for (int i = 0; i < entities.size(); i++) {
		entities[i].Update(elapsed);
	}
}
*/

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

	bool turn = false;

	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	//Matrix paddle1Matrix;
	//Matrix ballMatrix;
	Matrix viewModel;
	int enemyinf = 31;
	float playerPosition = -1.5f;

	std::vector<Entity> entities;

	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	//projectionMatrix.setOrthoProjection(-11, 11, -11, 11, -1.0f, 1.0f);
	glUseProgram(program.programID);

	float lastFrameTicks = 0.0f;

	GLuint font = LoadTexture("font1.png");

	bool gameWon = false;
	bool gameLose = false;

	int bulletIndex = 1;
	int MAX_BULLETS = 30;

	GLuint spriteSheetTexture1 = LoadTexture("characters_1.png");
	SheetSprite playerSprite = SheetSprite(&program, 52, 12, 8, spriteSheetTexture1, 0.5);
	SheetSprite projectileSprite = SheetSprite(&program, 85, 12, 8, spriteSheetTexture1, 0.2);
	SheetSprite enemySprite = SheetSprite(&program, 10, 12, 8, spriteSheetTexture1, 0.5);

	entities.push_back(Entity(Vector3{ 0, playerPosition, 0 }, Vector3{ 0,0,0 }, 1, &playerSprite, &program, ENTITY_PLAYER));
	for (int i = 0; i < MAX_BULLETS; i++) {
		entities.push_back(Entity(Vector3{ 9, 9, 0 }, Vector3{ 0,0,0 }, 1, &projectileSprite, &program, ENTITY_PROJECTILE));
	}
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 3; j++) {
			//entities.push_back(Entity(Vector3{ -2.0f+ (float) i*.8f , 0- ( float)j*.5f , 0 }, Vector3{ 0,0,0 }, 1, &enemySprite, &program,ENTITY_ENEMY));
			entities.push_back(Entity(Vector3{ -2.0f + (float)i*.8f , 1.7f - (float)j*.5f , 0 }, Vector3{ .05f*j+.1f,0,0 }, 1, &enemySprite, &program, ENTITY_ENEMY));
		}
	}
	//entities.push_back(Entity(Vector3{ -3 + (float) (i/2), 1.5f- (float) (j*.7), 0 }, Vector3{ 0,0,0 }, Vector3{ 1,1,1 }, 1, &enemySprite, &program));
	//entities.push_back(Entity(Vector3{ -3 + 1 , 1.5f -1 , 0 }, Vector3{ 0,0,0 }, Vector3{ 1,1,1 }, 1, &enemySprite, &program));
	//entities.push_back(Entity(Vector3{ -3 + 1.7f , 1.5f - 1.7f , 0 }, Vector3{ 0,0,0 }, Vector3{ 1,1,1 }, 1, &enemySprite, &program));
	bool pastStart = false;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*
	float angle = 0;
	int paddle1 = 0;
	int paddle2 = 0;
	const float paddleSpeed = 2;
	float disp=0;
	int ballDirection = 45;
	const float ballSpeed = .5;
	bool gameEnd = false;
	bool won;
	*/
	const Uint8 *keys;

/*#define MAX_BULLETS 30
	int bulletIndex = 0;
	//Entity bullets[MAX_BULLETS];
	for (int i = 0; i < MAX_BULLETS; i++) {
		bullets[i].x = -2000.0f;
	}

	GLuint spriteSheetTexture = LoadTexture("font1.png");
	SheetSprite mySprite = SheetSprite(spriteSheetTexture, 425.0f / 1024.0f, 468.0f / 1024.0f, 93.0f / 1024.0f, 84.0f / 1024.0f, 0.2);
*/
	//

	SDL_Event event;
	bool done = false;
	while (!done) {
		

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		if (!pastStart) {
			program.setModelMatrix(modelMatrix);
			program.setProjectionMatrix(projectionMatrix);
			program.setViewMatrix(viewModel);
			DrawText(&program, font, "CAVE INVADERS.", 0.3f, 0.001f, 1, -2.0f);
			DrawText(&program, font, "Press space to begin.", 0.3f, 0.001f, -1, -3);
		}
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			
			}else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					if (!pastStart) { pastStart = true; }
					else{ shootBullet(&entities, bulletIndex, MAX_BULLETS); }
				}
			}
		}
		if (!pastStart || gameWon || gameLose) {
			if (gameWon) DrawText(&program, font, "YOU WON!", 0.3f, 0.001f, -1, -3);
			if (gameLose) DrawText(&program, font, "GAME OVER.", 0.3f, 0.001f, -1, -3);
			SDL_GL_SwapWindow(displayWindow);
			glClear(GL_COLOR_BUFFER_BIT);
			continue;
		}
		keys = SDL_GetKeyboardState(NULL);
		if (keys[SDL_SCANCODE_RIGHT]) {
			entities[0].velocity.x = 1;
		}
		else if (keys[SDL_SCANCODE_LEFT]) {
			entities[0].velocity.x = -1;
		}
		//else if (keys[SDL_SCANCODE_SPACE]){
		//DrawText(&program, spriteSheetTexture, "Hello World", 0.3, 0.01);
		//shoot
		//shootBullet(&entities, bulletIndex, MAX_BULLETS);
		/*
		entities[1].position.x= entities[0].position.x;
		entities[1].position.y = entities[0].position.y;
		entities[1].position.z = entities[0].position.z;
		entities[1].velocity.y = .1;
		*/
		//}
		else {
			entities[0].velocity.x = 0;
		}
		//
		program.setModelMatrix(modelMatrix);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewModel);
		
		
		
		//mySprite.Draw();
		//SheetSprite mySprite = SheetSprite(spriteSheetTexture1, 425.0f / 1024.0f, 468.0f / 1024.0f, 93.0f / 1024.0f, 84.0f / 1024.0f, 0.2);
		//mySprite.Draw(&program);
		/*
		
		*/
		
		Update(entities, elapsed);
		/*for (int i = 31; i < 52; i++) {
			if (entities[1].canCollide) {
				entities[1].Collides(&entities[i]);
			}
		}*/
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i].type == ENTITY_PROJECTILE && entities[i].canCollide) {
				for (int j = 0; j < entities.size(); j++) {
					if (entities[j].type != ENTITY_PROJECTILE && entities[j].type != ENTITY_PLAYER && entities[j].canCollide) {
						//DrawText(&program, font, std::to_string(entities[i].position.y), 1, 0.01);
						entities[i].Collides(&entities[j]);
						//break;
					}
				}
			}
		}
		gameWon = true;
		for (int i = 31; i < 52; i++) {
			if (entities[i].canCollide) {
				if ((.1f+entities[i].position.x >3.55 && entities[i].velocity.x >0)|| (.1f + entities[i].position.x < -3.55 && entities[i].velocity.x < 0)){
					turn = true;
				}
				gameWon = false;
			}
		}
		if (turn == true) {
			for (int i = 31; i < 52; i++) {
				entities[i].position.y -= .24f;
				if (entities[i].position.y < -.94) {
					gameLose = true;
				}
				entities[i].velocity.x *= -1;
				entities[i].velocity.x<0?entities[i].velocity.x -= .2:entities[i].velocity.x += .2;
				turn = false;
			}
		}
		Render(entities, &program, elapsed);
		/*
		for (int i = entities.size()-1; i > -1; i--) {
			entities[i].Draw(elapsed);
		}
		*/
		
		Matrix model;
		model.identity();
		program.setModelMatrix(model);
		//DrawText(&program, font, std::to_string(entities[1].sprite->spriteHeight), 0.4f, 0.01f, -2, -2.5);
		//DrawText(&program, font, std::to_string(entities[51].position.y), 0.4f, 0.01f, -1, -2.5);
		/*enemyinf++;
		if (enemyinf > entities.size() - 1) {
			//exit(1);
			enemyinf = 31;
		}
		DrawText(&program, font, std::to_string(entities[enemyinf].position.y), 0.4f, 0.001f, -1, -2.5f);
		*/
		
		//-.94 lose condition
		
		//if(gameEnd){
		if(gameWon){
			glClearColor(0, 0, 1, 0);//Blue
			DrawText(&program, font, "YOU WON!", 0.3f, 0.001f, -1, -3);
		}
		if(gameLose){
			glClearColor(1, 0, 0, 0);//Red
			DrawText(&program, font, "GAME OVER.", 0.3f, 0.001f, -1, -3);
		}
		/*}else{
		glClearColor(0, 0, 0, 0);//Added
		}
		*/
		SDL_GL_SwapWindow(displayWindow);
		glClear(GL_COLOR_BUFFER_BIT);

	}

	SDL_Quit();
	return 0;
}
/*
int main(int argc, char *argv[])
{
	sdl_init(sdl_init_video);
	displaywindow = sdl_createwindow("my game", sdl_windowpos_centered, sdl_windowpos_centered, 640, 360, sdl_window_opengl);
	sdl_glcontext context = sdl_gl_createcontext(displaywindow);
	sdl_gl_makecurrent(displaywindow, context);
#ifdef _windows
	glewinit();
#endif
	//
	glviewport(0, 0, 640, 360);
	
	shaderprogram program(resource_folder"vertex.glsl", resource_folder"fragment.glsl");

	matrix projectionmatrix;
	matrix modelmatrix;
	matrix paddle1matrix;
	matrix ballmatrix;
	matrix viewmodel;


	std::vector<entity> entities;
	
	projectionmatrix.setorthoprojection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	gluseprogram(program.programid);

	float lastframeticks = 0.0f;

	glenable(gl_blend);
	glblendfunc(gl_src_alpha, gl_one_minus_src_alpha);
	
	float angle = 0;
	int paddle1 = 0;
	int paddle2 = 0;
	const float paddlespeed = 2;
	float disp=0;
	int balldirection = 45;
	const float ballspeed = .5;
	bool gameend = false;
	bool won;
	
	const uint8 *keys;

#define max_bullets 30
	int bulletindex = 0;
	//entity bullets[max_bullets];
	for (int i = 0; i < max_bullets; i++) {
		bullets[i].x = -2000.0f;
	}

	gluint spritesheettexture = loadtexture("sheet.png");
	sheetsprite mysprite = sheetsprite(spritesheettexture, 425.0f / 1024.0f, 468.0f / 1024.0f, 93.0f / 1024.0f, 84.0f /1024.0f, 0.2);

	//
	sdl_event event;
	bool done = false;
	while (!done) {

		float ticks = (float)sdl_getticks() / 1000.0f;
		float elapsed = ticks - lastframeticks;
		lastframeticks = ticks;

		while (sdl_pollevent(&event)) {
			if (event.type == sdl_quit || event.type == sdl_windowevent_close) {
				done = true;
			}
			keys = sdl_getkeyboardstate(null);
			if (keys[sdl_scancode_down]) {
				
				if (paddle1matrix.m[3][1] >= -1.1) {
					disp += -1 * paddlespeed*elapsed;
					paddle1matrix.translate(0.0f, (float)(-1 * paddlespeed*elapsed), 0.0f);
				}
			} else if (keys[sdl_scancode_up]) {
				if (paddle1matrix.m[3][1] <= 1.1){
					disp += 1* paddlespeed*elapsed;
					paddle1matrix.translate(0.0f, (float)(1 * paddlespeed*elapsed), 0.0f);
				}
				
			}
		}
		
		//int ch =(float) 1.9f-.5;//ceiling collision box height
		//
		int vd = sin(balldirection * 3.14 / 180)*ballspeed*elapsed; //vertical distance
		int hd = cos(balldirection * 3.14 / 180)*ballspeed*elapsed; //horizontal distance
		
		//if (ballmatrix.m[3][1] < 1.80f && vd + ballmatrix.m[3][1] >= 1.80f) {// overstep
		//	ballmatrix.setposition(1.80, 0, 0);
		//}
		
		if (!gameend) {
			if (ballmatrix.m[3][1] >= 1.80) {//collision ceiling
				balldirection == 45 ? balldirection = 315 : balldirection = 225;
			}
			else if (ballmatrix.m[3][0] >= 3.35 && (abs(ballmatrix.m[3][1]-paddle1matrix.m[3][1]) <= .8)) {//||(paddle1matrix.m[3][0] ballmatrix.m[3][0])){//collision paddle 1
				balldirection == 45 ? balldirection = 135 : balldirection = 225;
			}
			else if (ballmatrix.m[3][1] <= -1.80) {// && (balldirection >= 180 && balldirection <= 360)) {//collision floor
				balldirection == 225 ? balldirection = 135 : balldirection = 45;
			}
			else if (ballmatrix.m[3][0] <= -3.35 && (abs(ballmatrix.m[3][1]-paddle1matrix.m[3][1]) <= .8)){// && (balldirection >= 0 && balldirection <= 180)) {//collision paddle 2
				balldirection == 135 ? balldirection = 45 : balldirection = 315;
			}
			ballmatrix.translate(cos(balldirection* 3.14 / 180)*ballspeed*elapsed, sin(balldirection * 3.14 / 180)*ballspeed*elapsed, 0);
			if (ballmatrix.m[3][0] < -3.5){  
				gameend = true;
				won = true;
			}
			else if (ballmatrix.m[3][0] > 3.5) {
				gameend = true;
				won = false;
			}
		}
		else {
			ballmatrix.translate(cos(balldirection* 3.14 / 180)*ballspeed*elapsed, sin(balldirection * 3.14 / 180)*ballspeed*elapsed, 0);
		}
		
		//
		
		
		for (int i = 0; i < max_bullets; i++) {
			bullets[i].update(elapsed);
		}
		//
		//balldirection = 305;
		std::vector<float> vertices;
		
		//ball
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

		//ceiling
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

		//floor
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
		
		//paddle2
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
		
		//paddle1
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
		

		glvertexattribpointer(program.positionattribute, 2, gl_float, false, 0, vertices.data());
		glenablevertexattribarray(program.positionattribute);

		//float texcoords[] = { 0.0,1.0 ,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,
		//	0.0,1.0 ,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0,
		//	0.0,1.0 ,1.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0,0.0,0.0 };

	    //glvertexattribpointer(program.texcoordattribute, 2, gl_float, false, 0, texcoords);
		//glenablevertexattribarray(program.texcoordattribute);

		//modelmatrix.identity();
		program.setmodelmatrix(ballmatrix);
		gldrawarrays(gl_triangles, 0, 6);

		program.setmodelmatrix(modelmatrix);
		program.setprojectionmatrix(projectionmatrix);
		program.setviewmatrix(viewmodel);
		//gldrawarrays(gl_triangles, paddle1, 6);
		gldrawarrays(gl_triangles, 6, (vertices.size()/2)-18);
		
		
		program.setmodelmatrix(paddle1matrix);
		//gldrawarrays(gl_triangles, paddle2, 12);
		
		angle += elapsed;

		modelmatrix.setrotation(angle * -85 * 3.14159265359 / 180);
		program.setmodelmatrix(modelmatrix);
		
		gldrawarrays(gl_triangles, 12, 6);


		modelmatrix.setrotation(angle * 110 * 3.14159265359 / 180);
		program.setmodelmatrix(modelmatrix);
		
		gldrawarrays(gl_triangles, 6, 6);

		modelmatrix.identity();
		program.setmodelmatrix(modelmatrix);

		
		gldrawarrays(gl_triangles, 0, 6);

		gldisablevertexattribarray(program.positionattribute);
		gldisablevertexattribarray(program.texcoordattribute);
		
		//
		sdl_gl_swapwindow(displaywindow);
		
		/*
		if(gameend){
			if(won){
				glclearcolor(0, 0, 1, 0);//blue
			}
			else{
				glclearcolor(1, 0, 0, 0);//red
			}
		}else{
			glclearcolor(0, 0, 0, 0);//added
		}
		
		glclear(gl_color_buffer_bit);

	}

	sdl_quit();
	return 0;
}
*/