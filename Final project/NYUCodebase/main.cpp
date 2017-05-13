#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Entity.h"

//SheetSprite tileSprite, playerSprite, projectileSprite;
#define RAND_FLOAT -1+2*((float)rand())/RAND_MAX
#define MAX_TILES 4

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

void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing, float ydisp, float xdisp) {
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

void setup(std::vector<Entity>& entities, ShaderProgram& program) {
	GLuint tileText = LoadTexture("elementStone050.png");
	GLuint playerWalk1 = LoadTexture("alienGreen_walk1.png");
	GLuint playerWalk2 = LoadTexture("alienGreen_walk2.png");
	GLuint playerStand = LoadTexture("alienGreen_stand.png");

	GLuint player2Walk1 = LoadTexture("alienBlue_walk1.png");
	GLuint player2Walk2 = LoadTexture("alienBlue_walk2.png");

	

	float random = RAND_FLOAT;
	//playerStand,
	
	entities.push_back(Entity(Vector3{-1.0f,1.0f,0.0f}, Vector3{0.0f,0.0f,0.0f}, Vector3{ 0.5f,0.5f,0.5f }, 0.0f, std::vector<GLuint> {playerWalk2, playerWalk1}, &program,
		ENTITY_PLAYER, true,0.06f));
	entities.push_back(Entity(Vector3{ -1.0f,1.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.5f,0.5f,0.5f }, 0.0f, std::vector<GLuint> {player2Walk2, player2Walk1}, &program,
		ENTITY_PLAYER, true, 0.06f));
	entities.push_back(Entity(Vector3{ 0,0,0 }, Vector3{ 3.0f,0,0 }, Vector3{ 3.0f,3.0f,3.0f }, 0.0f, std::vector<GLuint> {tileText}, &program, ENTITY_TILE));
	for (int i = 0; i < MAX_TILES; i++) {
		entities.push_back(Entity(Vector3{ 0,(i+1.0f)*4.0f,0 }, Vector3{ 0.0f,0,0 }, Vector3{ 3.0f,3.0f,3.0f }, 0.0f, std::vector<GLuint> {tileText}, &program, ENTITY_TILE));
	}
	
	

	//entities.push_back(Entity(Vector3{ 0,0,0 }, Vector3{ 3.0f,0,0 }, Vector3{ 3.0f,3.0f,3.0f }, 0.0f, std::vector<GLuint> {tileText}, &program, ENTITY_TILE));
	//entities.push_back(Entity(Vector3{ 0,9,0 }, Vector3{ 0.0f,0,0 }, Vector3{ 1.0f,1.0f,1.0f }, 0.0f, std::vector<GLuint> {tileText}, &program, ENTITY_TILE));
	//entities.push_back(Entity(Vector3{ 0,9,0 }, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 2.0f,2.0f,2.0f }, 0.0f, std::vector<GLuint> {tileText}, &program, ENTITY_TILE));
	//entities.push_back(Entity(Vector3{3*2*.25f, -.7f,0.0f}, Vector3{-1.0f,0.0f,0.0f}, Vector3{1.1f,1.1f,1.1f}, 0.0f, std::vector<GLuint> {tileText}, &program, ENTITY_TILE));
}

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
	glViewport(0, 0, 640, 360);

	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	Matrix projectionMatrix;
	Matrix viewModel;
	Matrix modelMatrix;
	std::vector<Entity> entities;
	bool initialized = false;
	float lastFrameTicks = 0.0f;	
	const Uint8 *keys;
	SDL_Event event;
	bool done = false;
	
	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	//projectionMatrix.setOrthoProjection(-6, 6, -8, 8, -1.0f, 1.0f);
	glUseProgram(program.programID);
	setup(entities, program);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	Mix_Chunk *jumpSound;
	Mix_Music *music;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	jumpSound = Mix_LoadWAV("sfx.wav");
	music = Mix_LoadMUS("quietest_-_06_-_chime06.mp3");

	//Mix_PlayChannel(-1, jumpSound, 0);
	

	
	int next = 0;
	bool tileReset = false;
	bool pastStart = false;
	bool canJump = false;
	int cycleOut = 0;
	GLuint font = LoadTexture("font1.png");
	float points = 0;
	std::stringstream buffer;

	bool gameOver = false;

	GLuint backgroundText = LoadTexture("backgrounds.png");
	Entity background (Vector3{ 0,0,0.0f }, Vector3{ 0.0f,0,0 }, Vector3{ 7.10f,4,1 }, 0.0f, std::vector<GLuint> {backgroundText}, &program, ENTITY_ENEMY);

	while (!done) {
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		Mix_PlayMusic(music, -1);

		points += elapsed;
		//canJump = false;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					if (!pastStart) { pastStart = true; }

					//else { shootBullet(&entities, bulletIndex, MAX_BULLETS); }
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				 //exit
					Mix_FreeChunk(jumpSound);
					Mix_FreeMusic(music);
					SDL_Quit();
					return 0;
				}
			}
		}
		if (!pastStart) {
			program.setModelMatrix(modelMatrix);
			program.setProjectionMatrix(projectionMatrix);
			program.setViewMatrix(viewModel);
			DrawText(&program, font, " Space Runner ", 0.3f, 0.001f, 1, -2.0f);
			DrawText(&program, font, "Press space to begin.", 0.3f, 0.001f, -1, -3);
			SDL_GL_SwapWindow(displayWindow);
			glClear(GL_COLOR_BUFFER_BIT);
			continue;
		}
		if (gameOver) {
			program.setModelMatrix(modelMatrix);
			program.setProjectionMatrix(projectionMatrix);
			program.setViewMatrix(viewModel);
			DrawText(&program, font, " Game Over! ", 0.3f, 0.001f, 1, -2.0f);
			DrawText(&program, font, "Press escape to exit", 0.3f, 0.001f, -1, -3);
			SDL_GL_SwapWindow(displayWindow);
			glClear(GL_COLOR_BUFFER_BIT);
			continue;
		}

		tileReset = false;
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i].type == ENTITY_PLAYER) {//add enemy later
			//apply aceleration
				entities[i].velocity.y -= 1 * elapsed;
				//check if left screen then game over.
				if (
					(entities[i].position.x - (0.5*entities[i].scale.x) >= 3.55f) ||
					(entities[i].position.x + (0.5*entities[i].scale.x) <= -3.55f) ||
					(entities[i].position.y - (0.5*entities[i].scale.y) >= 2.0f) ||
					(entities[i].position.y + (0.5*entities[i].scale.y) <= -2.0f)
					) { //player dies
					entities[i].position = std::move(Vector3{ 0,i*-4.0f - 4.0f,0 });
					entities[i].velocity = std::move(Vector3{ 0,0,0 });
				}
			}
			//cycle in
			else if (entities[i].type == ENTITY_TILE) {
				if (!tileReset) {//
					if (entities[i].position.x - (0.5*entities[i].scale.x) >= 3.55f / 3 && cycleOut == 0) {//cycle in
						i + 1 >= entities.size() - 1 ? next = 2 : next = i + 1;
						if (entities[next].velocity.x == 0) next + 1 >= entities.size() - 1 ? next = 2 : next = i + 1;
						entities[next].position = std::move(Vector3{ -3,-2 + RAND_FLOAT,0.0f });
						entities[next].velocity = std::move(Vector3{ 3,0,0 });
						cycleOut = i;
					}
					if (entities[i].position.x - (0.5*entities[i].scale.x) >= 3.55f && cycleOut == i) {//cycle out
						entities[i].position = std::move(Vector3{ 0,i*4.0f,0 });
						entities[i].velocity = std::move(Vector3{ 0,0,0 });
						tileReset = true;
						cycleOut = 0;
					}
				}
			}
		}
		if (entities[0].position == Vector3{ 0,-4.0f,0 } && entities[1].position == Vector3{ 0,-8.0f,0 }){
			//Game Over
			gameOver = true;
		}

		keys=SDL_GetKeyboardState(NULL);
		switch (HandleInput(entities, keys, elapsed, canJump)) {
		case -1:
			SDL_Quit();
			return 0;
		case 1:
			Mix_PlayChannel(-1, jumpSound, 0);
			canJump = false;
		}
				
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewModel);
		
		
		glClear(GL_COLOR_BUFFER_BIT);
		program.setModelMatrix(modelMatrix);
		background.Animate(elapsed);

		if (Run(entities, elapsed, initialized, program)) {
			points += 10 * elapsed;
			canJump = true;
		}
		else {
			//glClearColor(0, 0, 1, 1);
		}
		
		buffer.precision(2);
		//sprintf_s(pointsStr," Points: %i",points );
		program.setModelMatrix(modelMatrix);
		buffer << " Points: " << std::fixed << points;
		DrawText(&program, font, buffer.str(), 0.3f, 0.001f, 1.6f, -2.0f);


		SDL_GL_SwapWindow(displayWindow);
		
		glClear(GL_COLOR_BUFFER_BIT);
		buffer.str("");
		
	}

	Mix_FreeChunk(jumpSound);
	Mix_FreeMusic(music);
	SDL_Quit();
	return 0;
}