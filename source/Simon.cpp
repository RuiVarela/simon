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

    if (m->base_pass.id == -1 || IsWindowResized())
    {
        //
        // Setup Base Pass
        //
        if (m->base_pass.id != -1)
        {
            UnloadRenderTexture(m->base_pass);
            m->base_pass.id = -1;
        }
        m->base_pass = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        SetTextureFilter(m->base_pass.texture, FILTER_BILINEAR);

        //
        // setup cell sizes
        //
        for (auto &cell : m->cells)
            cell.computeSizeParameters(GetScreenWidth(), GetScreenHeight());
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse_posiion = GetMousePosition();
        for (auto &cell : m->cells)
        {
            if (cell.inside(mouse_posiion))
            {
                logDbg("Simon", sfmt("Click: %d", cell.kind()));
            }
        }
    }
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
    float thickness = 0.01f;
    const Color border_color = OLD_LAVANDER;

    Vector2 v_line_start;
    v_line_start.x = cell.center().x;
    v_line_start.y = float(cell.center().y) - cell.radius();
    Vector2 v_line_end;
    v_line_end.x = cell.center().x;
    v_line_end.y = float(cell.center().y) + cell.radius();

    Vector2 h_line_start;
    h_line_start.x = float(cell.center().x) - cell.radius();
    h_line_start.y = cell.center().y;
    Vector2 h_line_end;
    h_line_end.x = float(cell.center().x) + cell.radius();
    h_line_end.y = cell.center().y;

    BeginScissorMode(cell.scissor().x, cell.scissor().y, cell.scissor().width, cell.scissor().height);
    {
        DrawCircle(cell.center().x, cell.center().y, cell.radius() * (1.0 + thickness), border_color);
        DrawCircleGradient(cell.center().x, cell.center().y, cell.radius(), cell.centerColor(), cell.color());
        DrawLineEx(h_line_start, h_line_end, cell.radius() * thickness, border_color);
        DrawLineEx(v_line_start, v_line_end, cell.radius() * thickness, border_color);
    }
    EndScissorMode();
}

void Simon::render()
{
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
    SetTraceLogLevel(LOG_DEBUG);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE /*| FLAG_MSAA_4X_HINT*/);

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