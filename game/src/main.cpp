#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "game.h"

const unsigned int TARGET_FPS = 50; //frames/second
float dt = 1.0f / TARGET_FPS; //seconds/frame, 1/50=0.02
float time = 0.0f;
constexpr float sliderIndent = 200.0f;
constexpr int uiFontSize = 20;
constexpr int uiPaddingSize = 10;
float x, y, angle, speed;

//Changes world state
void update()
{
    time += dt;
}

//Display world state
void draw()
{
    const char *t_LaunchPosX = "Launch Pos X";
    const char *t_LaunchPosY = "Launch Pos Y";
    const char *t_LaunchAngle = "Launch Angle";
    const char *t_LaunchSpeed = "Launch Speed";
    const char *t_X = TextFormat("%.2f", x);
    const char *t_Y = TextFormat("%.2f", y);
    const char *t_Angle = TextFormat("%.2f", angle);
    const char *t_Speed = TextFormat("%.2f", speed);

    BeginDrawing();
    ClearBackground(BLACK);
    //------------------------------------------------------
    //week 2
    DrawText("Wen Wen 101539831", 10, GetScreenHeight() - 30, 20, LIGHTGRAY);

    GuiSliderBar(Rectangle{sliderIndent, uiPaddingSize, GetScreenWidth() - 2 * sliderIndent, uiFontSize}, "", "", &x, sliderIndent, (float)GetScreenWidth() - sliderIndent);
    DrawText(t_LaunchPosX, (int)sliderIndent - MeasureText(t_LaunchPosX, uiFontSize) - uiPaddingSize, uiPaddingSize, uiFontSize, WHITE);
    DrawText(t_X, GetScreenWidth() - (int)sliderIndent + uiPaddingSize, uiPaddingSize, uiFontSize, WHITE);

    GuiSliderBar(Rectangle{sliderIndent, 2 * uiPaddingSize + uiFontSize, GetScreenWidth() - 2 * sliderIndent, uiFontSize}, "", "", &y, sliderIndent, (float)GetScreenHeight() - sliderIndent);
    DrawText(t_LaunchPosY, (int)sliderIndent - MeasureText(t_LaunchPosY, uiFontSize) - uiPaddingSize, 2 * uiPaddingSize + uiFontSize, uiFontSize, WHITE);
    DrawText(t_Y, GetScreenWidth() - (int)sliderIndent + uiPaddingSize, 2 * uiPaddingSize + uiFontSize, uiFontSize, WHITE);

    GuiSliderBar(Rectangle{sliderIndent, 3 * uiPaddingSize + 2 * uiFontSize, GetScreenWidth() - 2 * sliderIndent, uiFontSize}, "", "", &angle, 0, 360.0f);
    DrawText(t_LaunchAngle, (int)sliderIndent - MeasureText(t_LaunchAngle, uiFontSize) - uiPaddingSize, 3 * uiPaddingSize + 2 * uiFontSize, uiFontSize, WHITE);
    DrawText(t_Angle, GetScreenWidth() - (int)sliderIndent + uiPaddingSize, 3 * uiPaddingSize + 2 * uiFontSize, uiFontSize, WHITE);

    GuiSliderBar(Rectangle{sliderIndent, 4 * uiPaddingSize + 3 * uiFontSize, GetScreenWidth() - 2 * sliderIndent, uiFontSize}, "", "", &speed, 0, 400.0f);
    DrawText(t_LaunchSpeed, (int)sliderIndent - MeasureText(t_LaunchSpeed, uiFontSize) - uiPaddingSize, 4 * uiPaddingSize + 3 * uiFontSize, uiFontSize, WHITE);
    DrawText(t_Speed, GetScreenWidth() - (int)sliderIndent + uiPaddingSize, 4 * uiPaddingSize + 3 * uiFontSize, uiFontSize, WHITE);

    DrawCircleV({x, y}, 5.0f, RED);
    float endX = x + (float)cos(-angle * DEG2RAD) * speed;
    float endY = y + (float)sin(-angle * DEG2RAD) * speed;
    DrawLineV({x, y}, {endX, endY}, RED);
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