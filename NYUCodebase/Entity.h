#pragma once
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

#include "Matrix.h"
#include "ShaderProgram.h"
#include <vector>
#include <iostream>

enum EntityType { ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_PROJECTILE, ENTITY_TILE };

class SheetSprite {
public:

	SheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY, GLuint spriteSheetTexture, float size);
	void Draw();

	ShaderProgram *program;
	int index;
	int spriteCountX;
	int spriteCountY;
	GLuint spriteSheetTexture;
	float size;
	float spriteWidth;
	float spriteHeight;
};

void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing);
void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing, int ydisp, int xdisp);

class Vector3 {
public:
	Vector3(float x, float y, float z);
	float x;
	float y;
	float z;
};

class Entity {
	enum Boundry { BOTTOM, TOP, LEFT, RIGHT };
public:
	Entity(Vector3 position, Vector3 velocity, float rotation, SheetSprite* sprite, ShaderProgram* program, EntityType type, bool isStatic);
	void Reset();
	void Draw();
	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 size;
	float rotation;
	SheetSprite* sprite;
	ShaderProgram* program;
	EntityType type;
	bool canCollide;
	Matrix Model;
	bool isStatic;
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;

};
void Render(std::vector<Entity>& entities, ShaderProgram* program, float elapsed);

void UpdateAll(std::vector<Entity>& entities, float elapsed, int cycles);

int collisionDetection(Entity& myself, std::vector<Entity>& entities, int i);

void Updater(Entity& myself, float elapsed);

void UpdatePlayer(Entity& myself, float elapsed);

void Initialize(std::vector<Entity>& entities, float elapsed);

bool Collides(Entity& myself, Entity* collider);

void applyAcceleration(std::vector<Entity>& entities, float elapsed);