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

Vector2 playerPos = { 100, 100 }; // position of feet (bottom center)
const float PLR_SPEED = 5.f;
const float PLR_TEXTURE_SCALE = 5.f;

Texture2D spriteSheet;
int frameCount = 8;
int rowCount = 4;
int frameWidth;
int frameHeight;

Rectangle axeRec;
Texture2D axeSheet;
int axeFrameCount = 8;
int axeRowCount = 4;
int axeFrameWidth;
int axeFrameHeight;

Rectangle frameRec;
int currentFrame = 0;
int framesCounter = 0;
int framesSpeed = 8;
int frameRow = 0;

bool isAxeMode = false;

void InitPlayer()
{
    spriteSheet = LoadTexture("images/spritesheet_robo.png");
    frameWidth = spriteSheet.width / frameCount;
    frameHeight = spriteSheet.height / rowCount;
    frameRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };

    axeSheet = LoadTexture("images/axe.png");
    axeFrameWidth = axeSheet.width / axeFrameCount;
    axeFrameHeight = axeSheet.height / axeRowCount;
}

void UpdatePlayer()
{
    isAxeMode = IsKeyDown(KEY_E);

    if (isAxeMode)
    {
        framesCounter++;
		if (currentFrame >= axeFrameCount)
		{
			isAxeMode = false;
		}
        if (framesCounter >= (60 / (framesSpeed*1.5))) {
            framesCounter = 0;
            currentFrame = (currentFrame + 1);
        }
		axeRec = {
            (float)(currentFrame * axeFrameWidth),
            0.0f,
            (float)axeFrameWidth,
            (float)axeFrameHeight
        };
		axeRec.y = axeFrameHeight * frameRow;
    }
    else
    {
        if (IsKeyDown(KEY_W)) { playerPos.y -= PLR_SPEED; frameRow = 1; }
        if (IsKeyDown(KEY_A)) { playerPos.x -= PLR_SPEED; frameRow = 3; }
        if (IsKeyDown(KEY_S)) { playerPos.y += PLR_SPEED; frameRow = 0; }
        if (IsKeyDown(KEY_D)) { playerPos.x += PLR_SPEED; frameRow = 2; }

        frameRec.y = frameHeight * frameRow;

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)) {
            framesCounter++;
        }
        else {
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
}

void DrawPlayer()
{
    if (isAxeMode)
    {
        DrawTexturePro(axeSheet,
            axeRec,
            {
                playerPos.x - (axeRec.width * PLR_TEXTURE_SCALE) / 2.0f,
                playerPos.y - (axeRec.height * PLR_TEXTURE_SCALE) / 2.0f,
                axeRec.width * PLR_TEXTURE_SCALE,
                axeRec.height * PLR_TEXTURE_SCALE
            },
            { 0, 0 },
            0.f,
            WHITE);
    }
    else
    {
        DrawTexturePro(spriteSheet,
            frameRec,
            {
                playerPos.x - (frameRec.width * PLR_TEXTURE_SCALE) / 2.0f,
                playerPos.y - (frameRec.height * PLR_TEXTURE_SCALE) / 2.0f,
                frameRec.width * PLR_TEXTURE_SCALE,
                frameRec.height * PLR_TEXTURE_SCALE
            },
            { 0, 0 },
            0.f,
            WHITE);
    }
}

void UpdateDrawFrame();

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "raylib [core] example - consistent animation position");
    InitPlayer();

    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    #else
        SetTargetFPS(60);
        while (!WindowShouldClose())
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
