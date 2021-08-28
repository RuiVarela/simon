#pragma once

#include "Project.h"

namespace re
{
    class Button
    {
    public:
        Button(std::string resource);

        Button(Button const& noncopyable) = delete;
        Button& operator=(Button const& noncopyable) =delete;

        ~Button();

        void update(float base_scale);
        void setPosition(Vector2 position);

        int width();
        int height();

        void render();

    private:
        Texture2D m_texture;       
        float m_scale;
        int m_width;
        int m_height;
        bool m_pressed;
        Vector2 m_position;
    };

}