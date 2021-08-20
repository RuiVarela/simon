#pragma once

#include "Project.h"

namespace re {

    class Throttler {
    public:
        Throttler();

        bool throttle();
        void setMaxFps(int value);

    private:
        Int64 m_max_fps;
        Int64 m_frame_time;
        Int64 m_last_used_frame;
    };
    
}
