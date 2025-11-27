#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

RenderTexture buffer;
float radius = 10.0f;
int interpolate = 0;
Vector2 last_position;
Color colour = WHITE;

void draw(Color colour) {
    BeginTextureMode(buffer);
    BeginBlendMode(BLEND_CUSTOM);
    rlSetBlendFactors(RL_SRC_ALPHA, RL_ZERO, RL_FUNC_ADD);

    Vector2 start_position = GetMousePosition();
    start_position.y = buffer.texture.height - start_position.y;

    Vector2 end_position = interpolate ? last_position : start_position;

    float distance = fmaxf(1.0f, (Vector2Distance(start_position, end_position)));

    for (float i = 0.0f; i < distance; i += 1.0f) {
        DrawCircleV(Vector2Lerp(start_position, end_position, i / distance), radius, colour);
    }

    EndBlendMode();
    EndTextureMode();
    interpolate = 1;
    last_position = start_position;
}

int main(int argc, char** argv) {
    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_TRANSPARENT);
    InitWindow(0, 0, "Annotate");
    HideCursor();
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    buffer = LoadRenderTexture(GetRenderWidth(), GetRenderHeight());

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLANK);

        if (IsKeyPressed(KEY_LEFT_BRACKET)  && radius > 5.0f)   radius = powf(radius, 0.9f);
        if (IsKeyPressed(KEY_RIGHT_BRACKET) && radius < 100.0f) radius = powf(radius, 1.1f);

        if (IsKeyPressed(KEY_ONE))   colour = WHITE;
        if (IsKeyPressed(KEY_TWO))   colour = RED;
        if (IsKeyPressed(KEY_THREE)) colour = GREEN;
        if (IsKeyPressed(KEY_FOUR))  colour = BLUE;
        if (IsKeyPressed(KEY_FIVE))  colour = YELLOW;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))  draw(colour);
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) draw(BLANK);
        if (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && IsMouseButtonUp(MOUSE_BUTTON_RIGHT)) interpolate = 0;

        DrawTexture(buffer.texture, 0, 0, WHITE);
        DrawCircleLinesV(GetMousePosition(), radius + 1, BLACK);
        DrawCircleLinesV(GetMousePosition(), radius + 0, colour);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}