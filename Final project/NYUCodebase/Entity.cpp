#include "Entity.h"

bool testSATSeparationForEdge(float edgeX, float edgeY, const std::vector<Vector3> &points1, const std::vector<Vector3> &points2, Vector3 &penetration) {
	float normalX = -edgeY;
	float normalY = edgeX;
	float len = sqrtf(normalX*normalX + normalY*normalY);
	normalX /= len;
	normalY /= len;

	std::vector<float> e1Projected;
	std::vector<float> e2Projected;

	for (int i = 0; i < points1.size(); i++) {
		e1Projected.push_back(points1[i].x * normalX + points1[i].y * normalY);
	}
	for (int i = 0; i < points2.size(); i++) {
		e2Projected.push_back(points2[i].x * normalX + points2[i].y * normalY);
	}

	std::sort(e1Projected.begin(), e1Projected.end());
	std::sort(e2Projected.begin(), e2Projected.end());

	float e1Min = e1Projected[0];
	float e1Max = e1Projected[e1Projected.size() - 1];
	float e2Min = e2Projected[0];
	float e2Max = e2Projected[e2Projected.size() - 1];

	float e1Width = fabs(e1Max - e1Min);
	float e2Width = fabs(e2Max - e2Min);
	float e1Center = e1Min + (e1Width / 2.0);
	float e2Center = e2Min + (e2Width / 2.0);
	float dist = fabs(e1Center - e2Center);
	float p = dist - ((e1Width + e2Width) / 2.0);

	if (p >= 0) {
		return false;
	}

	float penetrationMin1 = e1Max - e2Min;
	float penetrationMin2 = e2Max - e1Min;

	float penetrationAmount = penetrationMin1;
	if (penetrationMin2 < penetrationAmount) {
		penetrationAmount = penetrationMin2;
	}

	penetration.x = normalX * penetrationAmount;
	penetration.y = normalY * penetrationAmount;

	return true;
}

bool penetrationSort(const Vector3 &p1, const Vector3 &p2) {
	return p1.length() < p2.length();
}

bool checkSATCollision(const std::vector<Vector3> &e1Points, const std::vector<Vector3> &e2Points, Vector3 &penetration) {
	std::vector<Vector3> penetrations;
	for (int i = 0; i < e1Points.size(); i++) {
		float edgeX, edgeY;

		if (i == e1Points.size() - 1) {
			edgeX = e1Points[0].x - e1Points[i].x;
			edgeY = e1Points[0].y - e1Points[i].y;
		}
		else {
			edgeX = e1Points[i + 1].x - e1Points[i].x;
			edgeY = e1Points[i + 1].y - e1Points[i].y;
		}
		Vector3 penetration;
		bool result = testSATSeparationForEdge(edgeX, edgeY, e1Points, e2Points, penetration);
		if (!result) {
			return false;
		}
		penetrations.push_back(penetration);
	}
	for (int i = 0; i < e2Points.size(); i++) {
		float edgeX, edgeY;

		if (i == e2Points.size() - 1) {
			edgeX = e2Points[0].x - e2Points[i].x;
			edgeY = e2Points[0].y - e2Points[i].y;
		}
		else {
			edgeX = e2Points[i + 1].x - e2Points[i].x;
			edgeY = e2Points[i + 1].y - e2Points[i].y;
		}
		Vector3 penetration;
		bool result = testSATSeparationForEdge(edgeX, edgeY, e1Points, e2Points, penetration);

		if (!result) {
			return false;
		}
		penetrations.push_back(penetration);
	}

	std::sort(penetrations.begin(), penetrations.end(), penetrationSort);
	penetration = penetrations[0];

	Vector3 e1Center;
	for (int i = 0; i < e1Points.size(); i++) {
		e1Center.x += e1Points[i].x;
		e1Center.y += e1Points[i].y;
	}
	e1Center.x /= (float)e1Points.size();
	e1Center.y /= (float)e1Points.size();

	Vector3 e2Center;
	for (int i = 0; i < e2Points.size(); i++) {
		e2Center.x += e2Points[i].x;
		e2Center.y += e2Points[i].y;
	}
	e2Center.x /= (float)e2Points.size();
	e2Center.y /= (float)e2Points.size();

	Vector3 ba;
	ba.x = e1Center.x - e2Center.x;
	ba.y = e1Center.y - e2Center.y;

	if ((penetration.x * ba.x) + (penetration.y * ba.y) < 0.0f) {
		penetration.x *= -1.0f;
		penetration.y *= -1.0f;
	}

	return true;
}
/*
SheetSprite::SheetSprite() {}
SheetSprite::SheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY, GLuint spriteSheetTexture)
	: program(program), index(index), spriteCountX(spriteCountX), spriteCountY(spriteCountY), spriteSheetTexture(spriteSheetTexture),
	spriteWidth((1.0 / (float)spriteCountX)), spriteHeight((1.0 / (float)spriteCountY)) {}

void SheetSprite::Draw() {
	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
	GLfloat texCoords[] = {
		u, v + spriteHeight,
		u + spriteWidth, v,
		u, v,
		u + spriteWidth, v,
		u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight
	};
	float vertices[] = {
		0.5f, 0.5f,
		-0.5f, 0.5f,
		-0.5f, -0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f
	};

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
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

float Vector3::length() const {
	return sqrtf((x*x) + (y*y));
}

Entity::Entity(Vector3 position, Vector3 velocity, Vector3 scale, float rotation, std::vector<GLuint> sprite,
	ShaderProgram* program, ENTITY_TYPE type, bool canCollide, float animateReset)
	:position(position), velocity(velocity), rotation(rotation), scale(scale),
	sprite(sprite), program(program), canCollide(canCollide), animateTime(0), animateReset(animateReset),
	type(type)
{
	Model.identity();
	Model.Translate(position.x, position.y, position.z);
	if (rotation>0) Model.Rotate(rotation);

	Model.Scale(scale.x, scale.y, scale.z);
}

void Entity::Draw(unsigned int i, const float* textWrapCoords) {
	//if (i > sprite.size() || i < 0) return;
	glBindTexture(GL_TEXTURE_2D, sprite[i]);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, textWrapCoords);
	glEnableVertexAttribArray(program->positionAttribute);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Animate(float elapsed) {
	program->setModelMatrix(Model);
	if (type == ENTITY_TILE) {
		
		float textWrapCoords[12];
		unsigned int j = 0;
		for (j = 0; j < 12; j++) {
			//if (j%2==0) {
			//	textWrapCoords[j] = texCoords[j] * scale.y;
			//}
			//else {
				textWrapCoords[j] = texCoords[j] * scale.x;
			//}
			
		}
		if (sprite.size() == 1) {
			Draw(0, textWrapCoords);
		}
		else {
			unsigned int i = 0;
			animateTime += elapsed;
			if (animateTime >= animateReset) animateTime = 0;
			i = animateTime / (animateReset / (sprite.size()));
			Draw(i, textWrapCoords);
		}
	}
	else {
		//DRAW
		if (sprite.size() == 1) {
			Draw(0);
		}
		else {
			unsigned int i = 0;
			animateTime += elapsed;
			if (animateTime >= animateReset) animateTime = 0;
			i = animateTime / (animateReset / (sprite.size()));
			Draw(i);
		}
	}
}
void Render(std::vector<Entity>& entities, ShaderProgram* program, float elapsed) {
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < entities.size(); i++) {
		entities[i].Animate(elapsed);
	}
}

void Entity::Update(float elapsed) {
	Model.identity();
	Model.Translate(position.x, position.y, position.z);
	Move(velocity, elapsed);
	Model.Rotate(rotation);
	Model.Scale(scale.x, scale.y, scale.z);
}

/*
bool fixedUpdate(std::vector<Entity>& entities, float elapsed) {
	bool collision = false;
	// 60 FPS (1.0f/60.0f)
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
	float fixedElapsed = elapsed;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		UpdateAll(entities,FIXED_TIMESTEP);
		if (detectCollision(entities)) return true;
	}
	UpdateAll(entities, fixedElapsed);
	if (detectCollision(entities)) return true;
}
*/
float lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}

void Entity::Move(float x, float y, float z) {
	position.x += x;
	position.y += y;
	position.z += z;
	Model.Translate(x, y, z);
}
void Entity::Move(const Vector3& xyz, float mult = 1.0f) {
	Move(xyz.x*mult, xyz.y*mult, xyz.z*mult);
}
Vector3 Vector3::operator *(const Matrix& model) const {
	Vector3 ret;
	ret.x = model.m[0][0] * x + model.m[1][0] * y + model.m[2][0] * z + model.m[3][0] * 1.0f;
	ret.y = model.m[0][1] * x + model.m[1][1] * y + model.m[2][1] * z + model.m[3][1] * 1.0f;
	ret.z = model.m[0][2] * x + model.m[1][2] * y + model.m[2][2] * z + model.m[3][2] * 1.0f;
	return ret;
}

bool handleCollision(std::vector<Entity>& entities, const Vector3& penetration, int i, int j) {
	switch (entities[i].type) {
		case ENTITY_PLAYER:
			if (entities[j].type == ENTITY_TILE) {
				entities[i].Move(penetration, 1.0f);
				entities[i].velocity.y = 0;
				//entities[j].Move(penetration, 0.0f);
				return true;
			}
			if (entities[j].type == ENTITY_PLAYER) {
				//entities[i].Move(penetration, 1.0f);
				//entities[j].Move(penetration, 1.0f);
				//entities[j].velocity.y = 0;
				return false;
			}
			break;
		case ENTITY_TILE:
			if (entities[j].type == ENTITY_PLAYER) {
				//entities[i].Move(penetration, 1.0f);
				entities[j].Move(penetration, 1.0f);
				entities[j].velocity.y = 0;
				return true;
			}
			else if (entities[j].type == ENTITY_TILE) {//not really supposed to happen
				entities[i].Move(penetration, 0.5f);
				entities[j].Move(penetration, -0.5f);
				return false;
			}
		//case ENTITY_ENEMY:
			//die
	}
	/*
	entities[i].Move(penetration, 1.0f);
	entities[j].Move(penetration, 0.0f);
	entities[i].Move(penetration, 0.5f);
	entities[j].Move(penetration, -0.5f);
	*/
}


bool detectCollision(std::vector<Entity>& entities) {
	Vector3 penetration;
	//std::vector<Vector3> dummy1, dummy2;
	bool collision = false;
	for (int i = 0; i < entities.size(); i++) {
		if (!entities[i].canCollide) continue;
		//if (i != 0)continue;//debug
		for (int j = 0; j < entities.size(); j++) {
			//checkSATCollision(const std::vector<Vector3> &e1Points, const std::vector<Vector3> &e2Points, Vector3 &penetration)
			if (i == j) continue;
			if (!entities[j].canCollide) continue;
			if (checkSATCollision(getWorldSpaceCoordinates(entities[i]), getWorldSpaceCoordinates(entities[j]), penetration)) {
				if (handleCollision(entities, penetration, i, j)) {
					collision = true;
				}
			}
			penetration = std::move(Vector3{ 0,0,0 });
		}
	}
	return collision;
}
std::vector<Vector3> getWorldSpaceCoordinates(const Entity& object) {
	std::vector<Vector3> myWorldSpace(6);
	std::vector<Vector3> myPoints = {
		Vector3(0.5f, 0.5f,0),
		Vector3(-0.5f, 0.5f, 0),
		Vector3(-0.5f, -0.5f, 0),
		Vector3(-0.5f, -0.5f, 0),
		Vector3(0.5f, -0.5f, 0),
		Vector3(0.5f, 0.5f,0)
	};
	for(int i=0;i<myPoints.size();i++){
		myWorldSpace[i] = myPoints[i] * object.Model;
	}
	return myWorldSpace;
}

bool Vector3::operator==(const Vector3& other) const {
	return (x == other.x) && (y == other.y) && (z == other.z);
}
bool UpdateAll(std::vector<Entity>& entities, float elapsed, int cycles) {
	bool collision = false;//debug
	for (int c = 0; c < cycles; c++) {
		for (int i = 0; i < entities.size(); i++) {
			entities[i].Update(elapsed);
		}
		collision = collision || detectCollision(entities);
	}
	return collision;
}

int HandleInput(std::vector<Entity>& entities, const Uint8*& keys, float elapsed, bool canJump ){
	float xspeed = 3.0f;
	float yspeed = 2.0f;
	int returnCode =0;

	if (keys[SDL_SCANCODE_ESCAPE]) {
		return -1;
	}

	if (keys[SDL_SCANCODE_RIGHT]) {
		entities[0].velocity.x = xspeed;
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		entities[0].velocity.x = -xspeed;
	}
	else {
		if (entities[0].velocity.x < 0) {
			if (entities[0].velocity.x + 5 * elapsed > 0) {
				entities[0].velocity.x = 0;
			}else {
				entities[0].velocity.x += 5 * elapsed;
			}
		}else if (entities[0].velocity.x > 0) {
			if (entities[0].velocity.x + 5 * elapsed < 0) {
				entities[0].velocity.x = 0;
			}
			else {
				entities[0].velocity.x -= 5 * elapsed;
			}
		}
		
	}

	if (keys[SDL_SCANCODE_D]) {
		entities[1].velocity.x = xspeed;
	}
	else if (keys[SDL_SCANCODE_A]) {
		entities[1].velocity.x = -xspeed;
	}
	else {
		if (entities[0].velocity.x < 0) {
			if (entities[0].velocity.x + 5 * elapsed > 0) {
				entities[0].velocity.x = 0;
			}
			else {
				entities[0].velocity.x += 5 * elapsed;
			}
		}
		else if (entities[0].velocity.x > 0) {
			if (entities[0].velocity.x + 5 * elapsed < 0) {
				entities[0].velocity.x = 0;
			}
			else {
				entities[0].velocity.x -= 5 * elapsed;
			}
		}

	}

	if (canJump) {
		if (keys[SDL_SCANCODE_UP]) {
			entities[0].velocity.y = yspeed;
			returnCode = 1;
		}
		else if (keys[SDL_SCANCODE_DOWN]) {
			entities[0].velocity.y = -yspeed;
		}
		else {
			//entities[0].velocity.y = 0;
		}
		if (keys[SDL_SCANCODE_W]) {
			entities[1].velocity.y = yspeed;
			returnCode = 1;
		}
		else if (keys[SDL_SCANCODE_S]) {
			entities[1].velocity.y = -yspeed;
		}
		else {
			//entities[0].velocity.y = 0;
		}
	}
	return returnCode;
}

bool Run(std::vector<Entity>& entities, float elapsed, bool& initialized, ShaderProgram& program) {
	bool collision = false; //debug
	if (!initialized) {
		initialized = true;
		Render(entities, &program, elapsed / 6);
	}
	else {
		collision = UpdateAll(entities, elapsed / 6, 6);
		Render(entities, &program, elapsed / 6);
	}
	return collision;
}