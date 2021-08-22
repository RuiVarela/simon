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
        Color const& color();

    private:
        struct Implementation;
        std::shared_ptr<Implementation> m;
    };

}