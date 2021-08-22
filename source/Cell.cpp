#include "Project.h"

namespace re
{
    struct Cell::Implementation
    {
        Implementation()
        {
        }
        Kind kind;

        Vector2 center;
        Rectangle scissor;
        float radius;

        Color center_color;
        Color color;
    };

    Cell::Cell(Kind kind)
    {
        m = std::make_shared<Implementation>();
        m->kind = kind;

        if (kind == Kind::CellTL)
        {
            m->center_color = CLITERAL(Color){192, 255, 255, 255};
            m->color = CLITERAL(Color){24, 154, 180, 255};
        }
        else if (kind == Kind::CellTR)
        {
            m->center_color = CLITERAL(Color){255, 255, 151, 255};
            m->color = CLITERAL(Color){248, 210, 16, 255};
        }
        else if (kind == Kind::CellBL)
        {
            m->center_color = CLITERAL(Color){255, 191, 157, 255};
            m->color = CLITERAL(Color){245, 23, 32, 255};
        }
        else if (kind == Kind::CellBR)
        {
            m->center_color = CLITERAL(Color){128, 255, 255, 255};
            m->color = CLITERAL(Color){47, 243, 224, 255};
        }
    }

    void Cell::computeSizeParameters(int screen_w, int screen_h)
    {
        m->scissor.width = screen_w / 2;
        m->scissor.height = screen_h / 2;
        m->scissor.x = 0;
        m->scissor.y = 0;

        m->center.x = screen_w / 2;
        m->center.y = screen_h / 2;
        m->radius = re::minimum(m->center.x, m->center.y) * 0.8f;

        if (kind() == Cell::CellTL)
        {
        }
        else if (kind() == Cell::CellTR)
        {
            m->scissor.x += m->scissor.width;
        }
        else if (kind() == Cell::CellBL)
        {
            m->scissor.y += m->scissor.height;
        }
        else if (kind() == Cell::CellBR)
        {
            m->scissor.x += m->scissor.width;
            m->scissor.y += m->scissor.height;
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

    Color const &Cell::centerColor()
    {
        return m->center_color;
    }

    Vector2 const &Cell::center()
    {
        return m->center;
    }

    Rectangle const &Cell::scissor()
    {
        return m->scissor;
    }

    float const &Cell::radius()
    {
        return m->radius;
    }

    Cell::Kind Cell::kind()
    {
        return m->kind;
    }

    bool Cell::inside(Vector2 position) 
    {
        bool inside = true;

        inside &= position.x >= m->scissor.x && position.x < (m->scissor.x + m->scissor.width);
        inside &= position.y >= m->scissor.y && position.y < (m->scissor.y + m->scissor.height);

        float d_x = position.x - center().x;
        float d_y = position.y - center().y;
        inside &= std::hypot(d_x, d_y) < radius();

        return inside;
    }
}