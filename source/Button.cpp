#include "Project.h"

namespace re
{
    Button::Button(std::string resource)
    {
        m_texture = LoadTexture(resource.c_str());
        m_scale = 1.0f;
        m_width = m_texture.width;
        m_height = m_texture.height;
        m_pressed = false;
        m_position.x = 0;
        m_position.y = 0;
    }

    Button::~Button()
    {
        UnloadTexture(m_texture);
    }

    void Button::update(float base_scale)
    {
        //
        // update sizes
        //
        m_scale = 1.0f;
        m_width = float(m_texture.width) * base_scale;
        m_height = float(m_texture.height) * base_scale;

        float max_w = GetScreenWidth() * 0.7;

        if (m_width > max_w)
            m_width = max_w;

        m_scale = re::minimum(float(m_width) / float(m_texture.width),
                              float(m_height) / float(m_texture.height));

        m_width = float(m_texture.width) * m_scale;
        m_height = float(m_texture.height) * m_scale;
    }

    void Button::setPosition(Vector2 position)
    {
        m_position = position;

        Vector2 m = GetMousePosition();

        bool inside = true;
        inside &= m.x >= position.x && m.x < (position.x + m_width);
        inside &= m.y >= position.y && m.y < (position.y + m_height);

        m_pressed = inside && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    }

    int Button::width()
    {
        return m_width;
    }

    int Button::height()
    {
        return m_height;
    }

    void Button::render()
    {
        float rotation = 0.0f;

        Color tint;
        tint.r = 255;
        tint.g = 255;
        tint.b = 255;
        tint.a = m_pressed ? 150 : 255;

        DrawTextureEx(m_texture, m_position, rotation, m_scale, tint);
    }
}