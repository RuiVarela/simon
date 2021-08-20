#pragma once

#include "Project.h"

namespace re
{

    class Simon
    {
    public:
        Simon();
        ~Simon();

        void run();

        void step();
        void update();
        void render();

    private:
        struct Implementation;
        std::shared_ptr<Implementation> m;
    };

}
