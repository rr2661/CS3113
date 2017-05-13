#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

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


const float vertices[] = {
	0.5f, 0.5f,
	-0.5f, 0.5f,
	-0.5f, -0.5f,
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f
};

/*const GLfloat texCoords[] = {

};*/
const float texCoords[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0 };
enum ENTITY_TYPE {ENTITY_PLAYER,ENTITY_TILE,ENTITY_ENEMY};
//enum CollideFlags{BOT,TOP,LEFT,RIGHT};

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
	Entity(Vector3 position, Vector3 velocity, Vector3 scale, float rotation,
		std::vector<GLuint> sprite, ShaderProgram* program, ENTITY_TYPE type, bool canCollide = true, float animateReset = 0.0f);
	void Animate(float elapsed);
	void Update(float elapsed);
	//bool Collides(Entity* collider);
	void Move(float x, float y, float z);
	void Move(const Vector3& xyz, float mult);
	Vector3 position;
	Vector3 velocity;
	Vector3 scale;
	float rotation;
	std::vector<GLuint> sprite;
	ShaderProgram* program;
	Matrix Model;
	bool canCollide;
	float animateTime;
	float animateReset;
	ENTITY_TYPE type;
private:
	void Draw(unsigned int i, const float* textWrapCoords = texCoords);
};

//bool fixedUpdate(std::vector<Entity>& entities, float elapsed);

//Functions that act on all entities
void Render(std::vector<Entity>& entities, ShaderProgram* program, float elapsed);
bool UpdateAll(std::vector<Entity>& entities, float elapsed, int cycles);

//Collision related functions
bool detectCollision(std::vector<Entity>& entities);
bool testSATSeparationForEdge(float edgeX, float edgeY, const std::vector<Vector3> &points1, const std::vector<Vector3> &points2, Vector3 &penetration);
bool checkSATCollision(const std::vector<Vector3> &e1Points, const std::vector<Vector3> &e2Points, Vector3 &penetration);
std::vector<Vector3> getWorldSpaceCoordinates(const Entity& object);
//std::vector<Vector3> getWorldSpaceVector(const Entity& object);//Debug

//Functions called in main
int HandleInput(std::vector<Entity>& entities, const Uint8*& keys, float elapsed, bool canJump);
bool Run(std::vector<Entity>& entities,float elapsed, bool& Initialized, ShaderProgram& program );