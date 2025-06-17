#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


const int WIDTH = 850;
const int HEIGHT = 450;

Rectangle player = {100, 100, 50, 50};
const float PLR_SPEED = 5.f;

Texture2D spriteSheet;
int frameCount = 8; // Number of frames in the sprite sheet
int rowCount = 4; // Number of rows of frames

int frameWidth;
int frameHeight;

Rectangle frameRec;
Vector2 position = { 200, 200 };

const float PLR_TEXTURE_SCALE = 5.f;

int currentFrame = 0;
int framesCounter = 0;
int framesSpeed = 8; // Animation speed: frames per second
int frameRow = 0;

void InitPlayer()
{
	spriteSheet = LoadTexture("images/axe.png");
	frameCount = 8; // 8 columns
	frameWidth = spriteSheet.width / frameCount;
	frameHeight = spriteSheet.height / rowCount; // 4 rows
	frameRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
}

void UpdatePlayer()
{
	if (IsKeyDown(KEY_W)) {player.y -= PLR_SPEED; frameRow = 1;}
	if (IsKeyDown(KEY_A)) {player.x -= PLR_SPEED; frameRow = 3;}
	if (IsKeyDown(KEY_S)) {player.y += PLR_SPEED; frameRow = 0;}
	if (IsKeyDown(KEY_D)) {player.x += PLR_SPEED; frameRow = 2;}
	frameRec.y = frameHeight * frameRow;

	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)) {
		framesCounter++;
	}
	else
	{
		framesCounter = 0;
		currentFrame = 0;
		frameRec.x = (float)currentFrame * frameWidth;
	}

	if (framesCounter >= (60 / framesSpeed)) {
		framesCounter = 0;
		currentFrame = (currentFrame + 1) % frameCount;
		frameRec.x = (float)currentFrame * frameWidth;
	}
}

void DrawPlayer()
{
	DrawTexturePro(spriteSheet,
		frameRec,
		{player.x, player.y, frameRec.width * PLR_TEXTURE_SCALE, frameRec.height * PLR_TEXTURE_SCALE},
		{0, 0},
		0.f,
	WHITE);
}

void UpdateDrawFrame();

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "raylib [core] example - basic window");
	InitPlayer();

	#if defined(PLATFORM_WEB)
    	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
	#else
		SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
		//--------------------------------------------------------------------------------------

		// Main game loop
		while (!WindowShouldClose())    // Detect window close button or ESC key
		{
			UpdateDrawFrame();
		}
	#endif

	CloseWindow();

	return 0;
}

void UpdateDrawFrame()
{
	UpdatePlayer();

	BeginDrawing();
	
	ClearBackground(BLACK);

	DrawPlayer();

	if (GuiButton({200, 200, 100, 30}, "#32# PRESS ME!"))
	{
		std::cout << "Pressed!\n";
	}

	EndDrawing();
}