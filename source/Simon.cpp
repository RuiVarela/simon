#include "Project.h"
#include "../vendor/raylib/raylib.h"

using namespace re;

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

void Simon::render()
{
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
}

void Simon::step()
{
    update();
    render();
}

void Simon::run()
{
    InitWindow(800, 450, "simon  ");

    while (!WindowShouldClose())
    { 
        step();
    }

    CloseWindow();
}