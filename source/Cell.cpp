#include "Project.h"

#define STATE_CHANGE_TIME 80

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

        bool animating;
        bool animating_up;
        ElapsedTimer timer;
        float base_radius;
        float target_radius;
        float radius;

        Color center_color;
        Color base_color;
        Color color;

        bool pressed;

        Tone tone;
    };

    Cell::Cell(Kind kind)
    {
        m = std::make_shared<Implementation>();
        m->kind = kind;
        m->pressed = false;


        if (kind == Kind::CellTL)
        {
            m->center_color = CLITERAL(Color){192, 255, 255, 255};
            m->base_color = CLITERAL(Color){24, 154, 180, 255};
        }
        else if (kind == Kind::CellTR)
        {
            m->center_color = CLITERAL(Color){255, 255, 151, 255};
            m->base_color = CLITERAL(Color){248, 210, 16, 255};
        }
        else if (kind == Kind::CellBL)
        {
            m->center_color = CLITERAL(Color){255, 191, 157, 255};
            m->base_color = CLITERAL(Color){245, 23, 32, 255};
        }
        else if (kind == Kind::CellBR)
        {
            m->center_color = CLITERAL(Color){128, 150, 255, 255};
            m->base_color = CLITERAL(Color){47, 140, 224, 255};
        }
        m->color = m->base_color;
    }

    void Cell::computeSizeParameters(int screen_w, int screen_h)
    {
        m->scissor.width = screen_w / 2;
        m->scissor.height = screen_h / 2;
        m->scissor.x = 0;
        m->scissor.y = 0;

        m->center.x = screen_w / 2;
        m->center.y = screen_h / 2;
        m->base_radius = re::minimum(m->center.x, m->center.y) * 0.8f;
        m->radius = m->base_radius;
        m->target_radius = m->base_radius;
        m->timer.restart();
        m->animating = false;

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

    void Cell::update()
    {
        //
        // Start Sound
        //
        if (!m->tone.started()) {

            m->tone.start(440);

        } else {
            m->tone.mute(!m->animating);
            m->tone.update();
        }


        // 
        // Animation
        //
        if (m->animating)
        {
            if (m->timer.hasExpired(STATE_CHANGE_TIME))
            {
                if (!m->pressed)
                {
                    if (m->animating_up)
                    {
                        m->animating_up = false;
                        m->timer.restart();
                    }
                    else
                    {
                        m->radius = m->base_radius;
                        m->color = m->base_color;
                        m->animating = false;
                    }
                }
            }
            else
            {
                float percentage = m->timer.elapsed() / float(STATE_CHANGE_TIME);
                percentage = re::clampTo(percentage, 0.0f, 1.0f);
                if (!m->animating_up)
                    percentage = (1.0f - percentage);

                m->radius = m->base_radius + (m->target_radius - m->base_radius) * percentage;

                m->color.a = m->base_color.a - (unsigned char)(100.0f * percentage);

            }
        }
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

    void Cell::setPressed(bool pressed)
    {
        if (m->pressed == pressed)
            return;

        if (pressed && !m->animating)
        {
            m->target_radius = m->base_radius * 1.05f;
            m->animating = true;
            m->animating_up = true;
            m->timer.restart();
        }

        m->pressed = pressed;
    }
}