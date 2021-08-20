#include "Project.h"

namespace re
{
    struct Cell::Implementation
    {
        Implementation()
        {
        }
        Kind kind;
    };

    Cell::Cell(Kind kind)
    {
        m = std::make_shared<Implementation>();
        m->kind = kind;
    }

    Cell::~Cell()
    {
        m.reset();
    }

    Cell::Kind Cell::kind()
    {
        return m->kind;
    }
}