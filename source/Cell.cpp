#include "Project.h"

namespace re
{
    struct Cell::Implementation
    {
        Implementation()
        {
        }
        Kind kind;
        Color color;
    };

    Cell::Cell(Kind kind)
    {
        m = std::make_shared<Implementation>();
        m->kind = kind;

        if (kind == Kind::CellTL)
        {
            m->color = CLITERAL(Color) { 24, 154, 180, 255 };
        }
        else if (kind == Kind::CellTR)
        {
            m->color = CLITERAL(Color) { 248, 210, 16, 255 };
        }
        else if (kind == Kind::CellBL)
        {
            m->color = CLITERAL(Color) { 245, 23, 32, 255 };
        }
        else if (kind == Kind::CellBR)
        {
            m->color = CLITERAL(Color) { 47, 243, 224, 255 };
        }
    }

    Cell::~Cell()
    {
        m.reset();
    }

    Color const &Cell::color()
    {
        return m->color;
    }

    Cell::Kind Cell::kind()
    {
        return m->kind;
    }
}