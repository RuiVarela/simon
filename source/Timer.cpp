// __!!rengine_copyright!!__ //

#include "Project.h"

#include <float.h>

#if defined( _MSC_VER )

#include <sys/types.h>
#include <fcntl.h>
#include <windows.h>
#include <winbase.h>

namespace re
{
	void Timer::start()
	{
		seconds_per_tick = 1.0;

		LARGE_INTEGER frequency;
		if(QueryPerformanceFrequency(&frequency))
		{
			seconds_per_tick /= Real64(frequency.QuadPart);
		}

		start_tick = getTick();
		initialize();
	}

	Timer::TickType Timer::getTick() const
	{
		LARGE_INTEGER qpc;
		if (QueryPerformanceCounter(&qpc))
		{
			return qpc.QuadPart;
		}
        return 0;
    }


} // namespace rengine

#else

#include <sys/time.h>

namespace re
{
	void Timer::start()
	{
		seconds_per_tick = (1.0 / 1000000.0);
		start_tick = getTick();
		initialize();
	}

	Timer::TickType Timer::getTick() const
	{
		struct timeval time_val;
		gettimeofday(&time_val, 0);

		return ((Timer::TickType)time_val.tv_sec) * 1000000 + (Timer::TickType) time_val.tv_usec;
	}

} // namespace rengine

#endif //WIN32



namespace re {

static Timer g_timer;

Int64 getCurrentMs()
{
    return Int64(g_timer.elapsedTime() * 1000.0);
}

DurationProbe::DurationProbe(size_t samples, std::string const& tag)
    :m_time_mean(samples, 0, true),
     m_starts_time_mean(samples, 0, true),
     TAG(tag)
{
    m_elased_timer.restart();
    m_start = std::chrono::high_resolution_clock::now();
    m_time_max = FLT_MIN;
    m_time_min = FLT_MAX;
}

DurationProbe::~DurationProbe()
{

}

void DurationProbe::setTag(const std::string &tag)
{
    TAG = tag;
}

void DurationProbe::setPrefix(const std::string &prefix)
{
    m_prefix = prefix;
}

void DurationProbe::Start()
{
    auto now = std::chrono::high_resolution_clock::now();
    float milliseconds = std::chrono::duration<float, std::milli>(now - m_start).count();
    m_starts_time_mean.accumulate(milliseconds);

    m_start = now;
}

void DurationProbe::Stop(int warningInterval)
{
    auto now = std::chrono::high_resolution_clock::now();
    float milliseconds = std::chrono::duration<float, std::milli>(now - m_start).count();
    float mean = m_time_mean.accumulateAndGet(milliseconds);

    if (milliseconds > m_time_max)
        m_time_max = milliseconds;

    if (milliseconds < m_time_min)
        m_time_min = milliseconds;

    if (m_elased_timer.hasExpired(warningInterval)) {
        float starts_mean = m_starts_time_mean.mean();
        float fps = 0.0f;
        if (starts_mean > 0.0f)
            fps = 1000.0f / starts_mean;

        logDbg(TAG, sfmt("%sStart-Stop duration: %.3f min: %.3f avg: %.3f max: %.3f <> Start-Start avg: %.3f fps: %.3f",
                m_prefix,
                milliseconds,
                m_time_min, mean, m_time_max,
                starts_mean, fps)
                );

        m_time_max = FLT_MIN;
        m_time_min = FLT_MAX;
        m_elased_timer.restart();
    }
}

}
