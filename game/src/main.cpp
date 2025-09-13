/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "game.h"
#include "raygui.h"

const unsigned int TARGET_FPS = 50; //frames/second
float dt = 1.0f / TARGET_FPS; //seconds/frame, 1/50=0.02
float time = 0.0f;
float x = 500.0f;
float y = 500.0f;
float frequency = 1.0f;
float amplitude = 100.0f;

//Changes world state
void update()
{
    time += dt;
    //frequency appeared twice to make the movement speed proportional to the frequency
    //which means Higher frequency = faster oscillation AND proportionally faster movement
    //if I don't want this proportionality then the second frequency is redundant
    //and I could change to: x = x + (float)(-sin(time * frequency)) * amplitude * dt;
    x = x + (float)(-sin(time * frequency)) * frequency * amplitude * dt;
    y = y + (float)(cos(time * frequency)) * frequency * amplitude * dt;
}

//Display world state
void draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
    //------------------------------------------------------
    //week 1
    DrawText("Wen Wen 101539831", 10, GetScreenHeight() - 60, 20, LIGHTGRAY);
    GuiSliderBar(Rectangle{10, (float)GetScreenHeight() - 30, (float)GetScreenWidth() - 50, 20}, "", TextFormat("%.2f", time), &time, 0, 240);
    //format string c++: <https://cplusplus.com/reference/cstdio/printf/>
    DrawText(TextFormat("FPS: %i, TIME: %6.2f", TARGET_FPS, time), GetScreenWidth() - 220, 10, 20, LIGHTGRAY); //6-width, .2-decimal numbers, f-float
    //if I drag the slider, the two circles behave differently according to time
    DrawCircle((int)x, (int)y, 60, RED);
    //DrawCircleV({x, y}, 60, RED); //accept float
    DrawCircle((int)(GetScreenWidth() / 2 + cos(time * frequency) * amplitude), (int)(GetScreenHeight() / 2 + sin(time * frequency) * amplitude), 60, GREEN);
    //------------------------------------------------------
    EndDrawing();
}

int main()
{
    InitWindow(InitialWidth, InitialHeight, "GAME2005 - Wen Wen 101539831");
    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose()) //stops only by clicking to close the window or by pressing the Esc key
    {
        //Loops TARGET_FPS times per second
        update();
        draw();
    }
    CloseWindow();
    return 0;
}