#pragma once

#include "Project.h"

namespace re
{

    class Simon
    {
    public:
        Simon();
        ~Simon();

        void setup();
        void run();
        void shutdown();

        void step();
        void update();
        bool processGameLogic();
        void checkPresses();

        void render();
        void renderBackground();
        void renderCell(Cell& cell);

        void restartGame();

    private:
        struct Implementation;
        std::shared_ptr<Implementation> m;
    };

}
