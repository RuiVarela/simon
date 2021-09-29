#pragma once

#include "Project.h"

namespace re
{

    class Tone
    {
    public:
        Tone();
        ~Tone();

        void start(float frequency);
        void stop();
        bool started();

        void mute(bool value);

        void update();

    private:
        float m_frequency;

        bool m_muted;

        bool m_started;

        // Init raw audio stream (sample rate: 22050, sample size: 16bit-short, channels: 1-mono)
        AudioStream m_stream;

        static int const m_samplerate = 44100;
        std::array<short, 4096> m_data;
        Int64 m_sample_count;
        double m_volume;
        Int64 m_volume_timestamp;

    };

}