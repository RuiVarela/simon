#include "Project.h"

namespace re {

Throttler::Throttler()
{
    m_last_used_frame = 12345678;
    setMaxFps(1000);
}

bool Throttler::throttle()
{
    Int64 current = getCurrentMs();
    Int64 frame = current / m_frame_time;

    if (frame != m_last_used_frame) {
        m_last_used_frame = frame;
        return false;
    }

    return true;
}

void Throttler::setMaxFps(int value)
{
    if (value <= 0) {
        value = 1;
    }

    m_max_fps = value;
    m_frame_time = 1000 / m_max_fps;
}

}
