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

        // Buffer for the single cycle waveform we are synthesizing
        short *m_data;

        // Frame buffer, describing the waveform when repeated over the course of a frame
        short *m_writeBuf;

        // Cursor to read and copy the samples of the sine wave buffer
        int m_readCursor ;

        // Computed size in samples of the sine wave
        int m_waveLength;

        int m_samples_to_filter_count;
        int m_samples_filtered;
    };

}