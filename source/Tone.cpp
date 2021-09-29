#include "Project.h"

namespace re
{
    Tone::Tone()
    {
        m_frequency = 440.0f;
        m_muted = false;
        m_started = false;
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

        m_frequency = frequency;

        SetAudioStreamBufferSizeDefault(m_data.size());

        m_stream = LoadAudioStream(m_samplerate, 16, 1);

        for (size_t i = 0; i != m_data.size(); ++i)
            m_data[i] = 0;
        m_sample_count = 0;

        PlayAudioStream(m_stream);

        m_volume = 0.0f;
        m_volume_timestamp = m_sample_count;
        mute(true);

        m_started = true;
    }

    void Tone::stop()
    {
        if (m_started)
        {
            UnloadAudioStream(m_stream); // Close raw audio stream and delete buffers from RAM
            m_started = false;
        }
    }

    void Tone::update()
    {
        // Refill audio stream if required
        if (IsAudioStreamProcessed(m_stream))
        {
            double wavelength = 22050 / double(m_frequency);
            double audio_step = 10.0 / double(m_samplerate);

            for (size_t i = 0; i != m_data.size(); ++i)
            {
                double clock = double(m_sample_count) / double(wavelength);
                double value = sin(2.0f * PI * clock) * 32000.0;

                if (m_muted && m_volume != 0.0)
                {
                    m_volume -= audio_step;
                    if (m_volume < 0.0)
                        m_volume = 0.0;
                }
                else if (!m_muted && m_volume != 1.0)
                {
                    m_volume += audio_step;
                    if (m_volume > 1.0)
                        m_volume = 1.0;
                }

                m_data[i] = short(value * m_volume);
                m_sample_count++;
            }

            UpdateAudioStream(m_stream, m_data.data(), m_data.size());
        }
    }

    void Tone::mute(bool value)
    {
        if (m_muted == value)
            return;

        m_muted = value;
        m_volume_timestamp = m_sample_count;
    }
}