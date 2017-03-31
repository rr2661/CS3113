#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Entity.h"

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

	bool turn = false;
	bool initialized = false;

	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewModel;
	int enemyinf = 31;
	float playerPosition = -1.5f;

	std::vector<Entity> entities;

	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	glUseProgram(program.programID);

	float lastFrameTicks = 0.0f;

	GLuint font = LoadTexture("font1.png");

	bool gameWon = false;
	bool gameLose = false;

	int bulletIndex = 1;
	int MAX_BULLETS = 30;


	GLuint spriteSheetTexture2 = LoadTexture("Tiles.png");
	GLuint spriteSheetTexture1 = LoadTexture("characters_1.png");

	SheetSprite tileSprite = SheetSprite(&program, 0, 18, 1, spriteSheetTexture2, 0.5);
	SheetSprite playerSprite = SheetSprite(&program, 10, 12, 8, spriteSheetTexture1, 0.5);
	SheetSprite projectileSprite = SheetSprite(&program, 85, 12, 8, spriteSheetTexture1, 0.5);

	entities.push_back(Entity(Vector3{ 0, 0, 0 }, Vector3{ 0,0,0 }, 1, &playerSprite, &program, ENTITY_PLAYER,false));

	for (int j = 0; j < 55; j++) {
		entities.push_back(Entity(Vector3{ j*2*.25f-1 , -1.2f , 0 }, Vector3{ 0,0,0 }, 1, &tileSprite, &program, ENTITY_TILE, true));
	}
	entities.push_back(Entity(Vector3{ 3 * 2 * .25f  , -.7f , 0 }, Vector3{ 0,0,0 }, 1, &projectileSprite, &program, ENTITY_PROJECTILE, true));

	bool pastStart = false;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	const Uint8 *keys;

	SDL_Event event;
	bool done = false;
	while (!done) {
		

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			
			}else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				}
			}
		}

		keys = SDL_GetKeyboardState(NULL);
		
		if (keys[SDL_SCANCODE_RIGHT]) {
			entities[0].velocity.x = 1;
		}
		else if (keys[SDL_SCANCODE_LEFT]) {
			entities[0].velocity.x = -1;
		}

		if (keys[SDL_SCANCODE_UP]) {
			entities[0].velocity.y = 1;
		}
		else if (keys[SDL_SCANCODE_DOWN]) {
			entities[0].velocity.y = -1;
		}

		
		program.setModelMatrix(modelMatrix);
		program.setProjectionMatrix(projectionMatrix);

		if (!initialized) {
			Initialize(entities, elapsed);
			initialized = true;
			Render(entities, &program, elapsed);
		}
		else {
			UpdateAll(entities, elapsed, 6);
			Render(entities, &program, elapsed);
		}
		Matrix model;
		model.identity();
		program.setModelMatrix(model);
	
		if(gameWon){
			glClearColor(0, 0, 1, 0);//Blue
			DrawText(&program, font, "YOU WON!", 0.3f, 0.001f, -1, -3);
		}
		if(gameLose){
			glClearColor(1, 0, 0, 0);//Red
			DrawText(&program, font, "GAME OVER.", 0.3f, 0.001f, -1, -3);
		}

		SDL_GL_SwapWindow(displayWindow);
		glClear(GL_COLOR_BUFFER_BIT);

	}

	SDL_Quit();
	return 0;
}