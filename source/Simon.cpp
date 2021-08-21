#include "Project.h"
#include "../vendor/raylib/raylib.h"

using namespace re;

#define MINT_CREAM  CLITERAL(Color){ 240, 247, 238, 255 } 
#define BEAU_BLUE  CLITERAL(Color){ 196, 215, 242, 255 } 
#define POWDER_BLUE  CLITERAL(Color){ 175, 222, 220, 255 } 
#define MORNING_BLUE  CLITERAL(Color){ 145, 168, 164, 255 } 
#define OLD_LAVANDER  CLITERAL(Color){ 119, 104, 113, 255 } 

struct Simon::Implementation
{
    Implementation() {

    }
    std::array<Cell, 4> cells { 
        Cell::CellTL, Cell::CellTR, 
        Cell::CellBL, Cell::CellBR };

};

Simon::Simon()
{
}

Simon::~Simon()
{
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

void Simon::renderCell(Cell& cell)
{
    int center_x = GetScreenWidth() / 2;
    int center_y = GetScreenHeight() / 2;
    float radius = re::minimum(center_x, center_y) * 0.8f;

    DrawCircle(center_x, center_y, radius, POWDER_BLUE); 
    DrawCircleLines(center_x, center_y, radius, OLD_LAVANDER);   
}

void Simon::render()
{
    BeginDrawing();
        renderBackground();
        renderCell(m->cells[0]);

        DrawText(re::sfmt("Screen Size %d %d", GetScreenWidth(), GetScreenHeight()).c_str(), 
                20, 20, 20, LIGHTGRAY);
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

    while (!WindowShouldClose())
    { 
        step();
    }

    CloseWindow();
}