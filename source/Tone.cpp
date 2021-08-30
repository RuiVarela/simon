#include "Project.h"

#define MAX_SAMPLES 512
#define MAX_SAMPLES_PER_UPDATE 4096

namespace re
{
    Tone::Tone()
    {
        m_frequency = 440.0f;
        m_muted = false;
        m_started = false;
        m_data = nullptr;
        m_writeBuf = nullptr;
    }

    Tone::~Tone()
    {
        stop();
    }

    bool Tone::started()
    {
        return m_started;
    }

    void Tone::start(float frequency)
    {
        stop();

        SetAudioStreamBufferSizeDefault(MAX_SAMPLES_PER_UPDATE);

        // Init raw audio stream (sample rate: 22050, sample size: 16bit-short, channels: 1-mono)
        m_stream = InitAudioStream(44100, 16, 1);

        // Buffer for the single cycle waveform we are synthesizing
        m_data = (short *)malloc(sizeof(short) * MAX_SAMPLES);

        // Frame buffer, describing the waveform when repeated over the course of a frame
        m_writeBuf = (short *)malloc(sizeof(short) * MAX_SAMPLES_PER_UPDATE);

        PlayAudioStream(m_stream); // Start processing stream buffer (no data loaded currently)

        m_frequency = frequency;

        // Cursor to read and copy the samples of the sine wave buffer
        m_readCursor = 0;

        // Computed size in samples of the sine wave
        m_waveLength = 1;

        // Rewrite the sine wave.
        // Compute two cycles to allow the buffer padding, simplifying any modulation, resampling, etc.
        {
            // Compute wavelength. Limit size in both directions.
            m_waveLength = (int)(22050 / frequency);
            if (m_waveLength > MAX_SAMPLES / 2)
                m_waveLength = MAX_SAMPLES / 2;
            if (m_waveLength < 1)
                m_waveLength = 1;

            // Write sine wave.
            for (int i = 0; i < m_waveLength * 2; i++)
            {
                m_data[i] = (short)(sinf(((2 * PI * (float)i / m_waveLength))) * 32000);
            }
        }

        m_muted = false;
        mute(true);
        m_samples_filtered = -1;

        m_started = true;
    }

    void Tone::stop()
    {
        if (m_started)
        {
            CloseAudioStream(m_stream); // Close raw audio stream and delete buffers from RAM
            m_started = false;
        }

        if (m_data)
        {
            free(m_data);
        }

        if (m_writeBuf)
        {
            free(m_writeBuf);
        }
    }

    void Tone::update()
    {
        // Refill audio stream if required
        if (IsAudioStreamProcessed(m_stream))
        {
            // Synthesize a buffer that is exactly the requested size
            int writeCursor = 0;

            while (writeCursor < MAX_SAMPLES_PER_UPDATE)
            {
                // Start by trying to write the whole chunk at once
                int writeLength = MAX_SAMPLES_PER_UPDATE - writeCursor;

                // Limit to the maximum readable size
                int readLength = m_waveLength - m_readCursor;

                if (writeLength > readLength)
                    writeLength = readLength;

                // avoids popings on audio by interpolating a few frames on the beginning and end
                if (m_samples_filtered > -1)
                {
                    for (int i = 0; i != writeLength; ++i)
                    {
                        float percent = float(m_samples_filtered) / float(m_samples_to_filter_count);
                        percent = re::clampTo(percent, 0.0f, 1.0f);
                        if (m_muted)
                            percent = 1.0f - percent;

                        float value = *(m_data + m_readCursor + i);
                        value *= percent;

                        *(m_writeBuf + writeCursor + i) = short(value);

                        m_samples_filtered += 1;
                    }

                    // set as done
                    if (m_samples_filtered >= m_samples_to_filter_count)
                        m_samples_filtered = -1;
                }
                else
                {
                    if (m_muted)
                    {
                        // Write zeros - mute it
                        memset(m_writeBuf + writeCursor, 0, writeLength * sizeof(short));
                    }
                    else
                    {
                        // Write the slice
                        memcpy(m_writeBuf + writeCursor, m_data + m_readCursor, writeLength * sizeof(short));
                    }
                }

                // Update cursors and loop audio
                m_readCursor = (m_readCursor + writeLength) % m_waveLength;
                writeCursor += writeLength;
            }

            // Copy finished frame to audio stream
            UpdateAudioStream(m_stream, m_writeBuf, MAX_SAMPLES_PER_UPDATE);
        }
    }

    void Tone::mute(bool value)
    {
        if (m_muted == value)
            return;

        m_muted = value;

        m_samples_filtered = 0;
        m_samples_to_filter_count = MAX_SAMPLES_PER_UPDATE * 2;
        //SetAudioStreamVolume(m_stream, m_muted ? 0.0f : 1.0f);
    }
}