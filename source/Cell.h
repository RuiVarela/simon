#pragma once

#include "Project.h"

namespace re
{

    class Cell
    {
    public:
        enum Kind
        {
            CellTL,
            CellTR,
            CellBL,
            CellBR
        };

        Cell(Kind kind);
        ~Cell();

        Kind kind();
        Color const& centerColor();
        Color const& color();

        Vector2 const& center();
        Rectangle const& scissor();
        float const& radius();

        void update();
        bool inside(Vector2 position);
        void setPressed(bool pressed);
        bool isPressed();

        void computeSizeParameters(int screen_w, int screen_h);

    private:
        struct Implementation;
        std::shared_ptr<Implementation> m;
    };

}