#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Entity.h"

SheetSprite tileSprite, playerSprite, projectileSprite;

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

void setup(std::vector<Entity>& entities, ShaderProgram& program) {
	GLuint spriteSheetTexture2 = LoadTexture("Tiles.png");
	GLuint spriteSheetTexture1 = LoadTexture("characters_1.png");

	tileSprite = SheetSprite(&program, 0, 18, 1, spriteSheetTexture2);
	playerSprite = SheetSprite(&program, 10, 12, 8, spriteSheetTexture1);
	projectileSprite = SheetSprite(&program, 85, 12, 8, spriteSheetTexture1);

	entities.push_back(Entity(Vector3{-2.0f,2.0f,0.0f}, Vector3{0.0f,0.0f,0.0f}, Vector3{ 0.5f,0.5f,0.5f }, 0.9f, &playerSprite, &program));

	//for (int j = 0; j < 55; j++) {
	entities.push_back(Entity(Vector3{0.0f,0.0f,0.0f}, Vector3{0.0f,0.0f,0.0f}, Vector3{1.1f,1.1f,1.1f}, 5.0f, &tileSprite, &program));
	//}

	entities.push_back(Entity(Vector3{3*2*.25f, -.7f,0.0f}, Vector3{0.0f,0.0f,0.0f}, Vector3{1.5f,1.5f,1.5f}, 1.0f, &projectileSprite, &program));
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
	std::vector<Entity> entities;
	bool initialized = false;
	float lastFrameTicks = 0.0f;	
	const Uint8 *keys;
	SDL_Event event;
	bool done = false;
	
	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	glUseProgram(program.programID);

	setup(entities, program);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	while (!done) {
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		keys=SDL_GetKeyboardState(NULL);
		HandleInput(entities, keys);
				
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewModel);

		if (Run(entities, elapsed, initialized, program)) {
			glClearColor(0, 0, 0, 0);
		}
		else {
			glClearColor(.1, 0, 1, 1);
		}
		
		SDL_GL_SwapWindow(displayWindow);
		//glClear(GL_COLOR_BUFFER_BIT);

	}

	SDL_Quit();
	return 0;
}