#include "Project.h"
#include "../vendor/raylib/raylib.h"

using namespace re;

#define MINT_CREAM \
    CLITERAL(Color) { 240, 247, 238, 255 }
#define BEAU_BLUE \
    CLITERAL(Color) { 196, 215, 242, 255 }
#define POWDER_BLUE \
    CLITERAL(Color) { 175, 222, 220, 255 }
#define MORNING_BLUE \
    CLITERAL(Color) { 145, 168, 164, 255 }
#define OLD_LAVANDER \
    CLITERAL(Color) { 119, 104, 113, 255 }

struct Simon::Implementation
{
    Implementation()
    {
        base_pass.id = -1;
    }

    RenderTexture2D base_pass;

    std::array<Cell, 4> cells{
        Cell::CellTL, Cell::CellTR,
        Cell::CellBL, Cell::CellBR};
};

Simon::Simon()
{
    m = std::make_shared<Implementation>();
}

Simon::~Simon()
{
    m.reset();
}

void Simon::update()
{
}

void Simon::renderBackground()
{
    ClearBackground(RAYWHITE);

    Rectangle screen;
    screen.x = 0;
    screen.y = 0;
    screen.width = GetScreenWidth();
    screen.height = GetScreenHeight();

    Color c_start = RAYWHITE;
    Color c_mid = MINT_CREAM;
    Color c_end = BEAU_BLUE;

    DrawRectangleGradientEx(screen, c_mid, c_start, c_mid, c_end);
}

void Simon::renderCell(Cell &cell)
{
    Rectangle scisor;
    scisor.width = GetScreenWidth() / 2;
    scisor.height = GetScreenHeight() / 2;
    scisor.x = 0;
    scisor.y = 0;

    int center_x = GetScreenWidth() / 2;
    int center_y = GetScreenHeight() / 2;
    float radius = re::minimum(center_x, center_y) * 0.8f;

    if (cell.kind() == Cell::CellTL)
    {
    }
    else if (cell.kind() == Cell::CellTR)
    {
        scisor.x += scisor.width;
    }
    else if (cell.kind() == Cell::CellBL)
    {
        scisor.y += scisor.height;
    }
    else if (cell.kind() == Cell::CellBR)
    {
        scisor.x += scisor.width;
        scisor.y += scisor.height;
    }

    float thickness = 0.015f;
    const Color border_color = OLD_LAVANDER;

    Vector2 v_line_start;
    v_line_start.x = center_x;
    v_line_start.y = float(center_y) - radius;
    Vector2 v_line_end;
    v_line_end.x = center_x;
    v_line_end.y = float(center_y) + radius;

    Vector2 h_line_start;
    h_line_start.x = float(center_x) - radius;
    h_line_start.y = center_y;
    Vector2 h_line_end;
    h_line_end.x = float(center_x) + radius;
    h_line_end.y = center_y;

    BeginScissorMode(scisor.x, scisor.y, scisor.width, scisor.height);
    {
        DrawCircle(center_x, center_y, radius * (1.0 + thickness), border_color);
        DrawCircle(center_x, center_y, radius, cell.color());
        DrawLineEx(h_line_start, h_line_end, radius * thickness, border_color);
        DrawLineEx(v_line_start, v_line_end, radius * thickness, border_color);
    }
    EndScissorMode();
}

void Simon::render()
{
    //
    // Setup Base Pass
    //
    if (m->base_pass.id == -1 || IsWindowResized())
    {
        if (m->base_pass.id != -1)
        {
            UnloadRenderTexture(m->base_pass);
            m->base_pass.id = -1;
        }
        m->base_pass = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        SetTextureFilter(m->base_pass.texture, FILTER_BILINEAR); // Texture scale filter to use
    }

    //
    // Render Base Pass
    //
    BeginTextureMode(m->base_pass);
    {
        renderBackground();
        for (auto &cell : m->cells)
        {
            renderCell(cell);
        }

        DrawText(re::sfmt("Screen Size %d %d", GetScreenWidth(), GetScreenHeight()).c_str(),
                 20, 20, 20, LIGHTGRAY);
    }
    EndTextureMode();

    //
    // Render the texture to screen
    //
    BeginDrawing();
    {
        Rectangle src{0.0f, 0.0f, (float)m->base_pass.texture.width, -(float)m->base_pass.texture.height};
        Rectangle dst{0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};
        Vector2 origin{0, 0};

        DrawTexturePro(m->base_pass.texture, src, dst, origin, 0.0f, WHITE);
    }
    EndDrawing();
}

void Simon::step()
{
    update();
    render();
}

void Simon::run()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Dear Simon");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        step();
    }

    if (m->base_pass.id != -1)
    {
        UnloadRenderTexture(m->base_pass);
        m->base_pass.id = -1;
    }

    CloseWindow();
}