#include "Entity.h"

SheetSprite::SheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY, GLuint spriteSheetTexture, float size)
	: program(program), index(index), spriteCountX(spriteCountX), spriteCountY(spriteCountY), spriteSheetTexture(spriteSheetTexture), size(size),
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

	float aspect = 1;
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
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 2);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}
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
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 2);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}
Vector3::Vector3(float x, float y, float z)
	:x(x), y(y), z(z) {
}
Entity::Entity(Vector3 position, Vector3 velocity, float rotation, SheetSprite* sprite, ShaderProgram* program, EntityType type, bool isStatic)
	:position(position), velocity(velocity), size(std::move(Vector3{ sprite->size,sprite->size,0 })), rotation(rotation),
	sprite(sprite), program(program), type(type), canCollide(type != ENTITY_PROJECTILE), isStatic(isStatic),
	collidedTop(false), collidedBottom(false), collidedLeft(false) ,collidedRight(false), acceleration(Vector3(0,-1,0))
{}
void Entity::Draw() {
	if (type == ENTITY_PLAYER) {
		Matrix asd;
		asd.setPosition(position.x, position.y, position.z);
		program->setModelMatrix(asd);
		sprite->Draw();
	}
	else {
		program->setModelMatrix(Model);
		sprite->Draw();
	}
}
void Render(std::vector<Entity>& entities, ShaderProgram* program, float elapsed) {
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < entities.size(); i++) {
		entities[i].Draw();
	}
}

void Entity::Reset() {
	position = std::move(Vector3{ 9, 9, 0 });
	velocity.y = 0;
}

void Updater(Entity& myself, float elapsed) {
	//generate model matrix
	myself.position.x += myself.velocity.x * elapsed;
	myself.position.y += myself.velocity.y * elapsed;
	myself.position.z += myself.velocity.z * elapsed;
	myself.Model.setPosition(myself.position.x, myself.position.y, myself.position.z);
}
void UpdatePlayer(Entity& myself, float elapsed) {
	//generate view matrix
	myself.Model.identity();
	myself.position.x += myself.velocity.x * elapsed;
	myself.position.y += myself.velocity.y * elapsed;
	myself.position.z += myself.velocity.z * elapsed;
	myself.Model.Translate(-myself.position.x, -myself.position.y, -myself.position.z);
}

bool Collides(Entity& myself, Entity* collider) {
	float myWidth = .19f;
	float myHeight = .25f;
	float theirWidth = .19f;
	float theirHeight = .25f;

	if (!(
		(myself.position.y - myHeight > collider->position.y + theirHeight) ||
		(myself.position.y + myHeight < collider->position.y - theirHeight) ||
		(myself.position.x - myHeight > collider->position.x + theirWidth) ||
		(myself.position.x + myHeight < collider->position.x - theirWidth)
		)) {

		myself.collidedBottom = true;
		return true;
	}
	return false;
}

int collisionDetection(Entity& myself,std::vector<Entity>& entities, int i) {

	if (!myself.type == ENTITY_TILE) return 0;
	for (int j = 0; j < entities.size(); j++) {
		if (j == i) continue;// you cannot collide with yourself
		if (Collides(myself, &entities[j])) return j;
	}
	return 0;
}
float lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}
void Initialize(std::vector<Entity>& entities, float elapsed) {
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i].type == ENTITY_PLAYER) {
			UpdatePlayer(entities[i], 1);
			entities[i].program->setViewMatrix(entities[i].Model);
		}
		else {
			Updater(entities[i], 1);
		}
	}
}
void rolldback(std::vector<Entity>& entities, float elapsed, int cycles) {
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i].type == ENTITY_PLAYER) {
			for (int j = 0; j < cycles; j++) {
				entities[i].velocity.x += (entities[i].acceleration.x * elapsed);
				entities[i].velocity.y += (entities[i].acceleration.y * elapsed);	
				entities[i].velocity.z += (entities[i].acceleration.z * elapsed);
				entities[i].velocity.x = lerp(entities[i].velocity.x, 0.0f, elapsed*1.9);
				entities[i].Model.identity();
				Vector3 old(entities[i].position.x, entities[i].position.y, entities[i].position.z);
				entities[i].position.x += entities[i].velocity.x * elapsed;
				entities[i].position.y += entities[i].velocity.y * elapsed;
				entities[i].position.z += entities[i].velocity.z * elapsed;
				int collider = collisionDetection(entities[i], entities, i);
				if (collider != 0 && entities[collider].type != ENTITY_PROJECTILE) {
					int myBot = entities[i].position.y - entities[i].sprite->spriteHeight;
					int theirTop = entities[collider].position.y + entities[collider].sprite->spriteHeight;
					entities[i].position.y += theirTop - myBot +.01;
					entities[i].collidedBottom = false;
					entities[i].velocity.y = 0;
					entities[i].Model.Translate(-entities[i].position.x, -entities[i].position.y, -entities[i].position.z);
					entities[i].program->setViewMatrix(entities[i].Model);
				}
				else if (entities[collider].type == ENTITY_PROJECTILE) {
					entities[collider].Reset();
					entities[i].Model.Translate(-entities[i].position.x, -entities[i].position.y, -entities[i].position.z);
					entities[i].program->setViewMatrix(entities[i].Model);
				}else{
					entities[i].Model.Translate(-entities[i].position.x, -entities[i].position.y, -entities[i].position.z);
					entities[i].program->setViewMatrix(entities[i].Model);
				}
			}
			
		}
		else {
			for (int j = 0; j < cycles; j++) {
				Updater(entities[i], elapsed / cycles);
			}
		}
	}
}
void applyAcceleration(std::vector<Entity>& entities, float elapsed){
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i].type != ENTITY_PLAYER) continue;
		entities[i].velocity.x += (entities[i].acceleration.x /elapsed);
		entities[i].velocity.y += (entities[i].acceleration.y /elapsed);
		entities[i].velocity.z += (entities[i].acceleration.z /elapsed);
	}
}
void UpdateAll(std::vector<Entity>& entities, float elapsed, int cycles) {
	rolldback(entities, elapsed, cycles);
}