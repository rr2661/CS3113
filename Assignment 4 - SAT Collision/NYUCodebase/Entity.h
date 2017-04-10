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
#include <algorithm>

class SheetSprite {
public:
	SheetSprite();
	SheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY, GLuint spriteSheetTexture);
	void Draw();
	ShaderProgram* program;
	int index;
	int spriteCountX;
	int spriteCountY;
	GLuint spriteSheetTexture;
	float size;
	float spriteWidth;
	float spriteHeight;
};

class Vector3 {
public:
	Vector3(float x, float y, float z);
	Vector3 ();
	float length() const;
	Vector3 operator *(const Matrix& Model) const;
	bool operator ==(const Vector3& other) const;
	float x;
	float y;
	float z;
};

class Entity {
public:
	//Entity(Vector3 position, Vector3 velocity, Vector3 scale, float rotation, SheetSprite* sprite, ShaderProgram* program);
	Entity(Vector3 position, Vector3 velocity, Vector3 scale, float rotation, SheetSprite* sprite, ShaderProgram* program, bool canCollide = true);
	void Draw();
	void Update(float elapsed);
	void Move(float x, float y, float z);
	void Move(const Vector3& xyz, float mult);
	Vector3 position;
	Vector3 velocity;
	Vector3 scale;
	float rotation;
	SheetSprite* sprite;
	ShaderProgram* program;
	Matrix Model;
	bool canCollide;
};

//Functions that act on all entities
void Render(std::vector<Entity>& entities, ShaderProgram* program, float elapsed);
bool UpdateAll(std::vector<Entity>& entities, float elapsed, int cycles);

//Collision related functions
bool testSATSeparationForEdge(float edgeX, float edgeY, const std::vector<Vector3> &points1, const std::vector<Vector3> &points2, Vector3 &penetration);
bool checkSATCollision(const std::vector<Vector3> &e1Points, const std::vector<Vector3> &e2Points, Vector3 &penetration);
std::vector<Vector3> getWorldSpaceCoordinates(const Entity& object);
//std::vector<Vector3> getWorldSpaceVector(const Entity& object);//Debug

//Functions called in main
void HandleInput(std::vector<Entity>& entities, const Uint8*& keys);
bool Run(std::vector<Entity>& entities,float elapsed, bool& Initialized, ShaderProgram& program );