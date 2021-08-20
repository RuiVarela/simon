#include "Project.h"

namespace re {

Worker::Worker()
{
    m_pumping = false;

    setTag("Worker_noname");
    setSleepMs(1);
}

Worker::~Worker()
{
    stop();
}

void Worker::setTag(std::string TAG)
{
    this->TAG = TAG;
}

void Worker::setSleepMs(int millis)
{
    m_sleep_milliseconds = millis;
}

void Worker::start()
{
    std::unique_lock<std::recursive_mutex> lock(m_access_mutex);

    stop();

    m_pumping = true;
    m_runner = std::thread(&Worker::pump, this);
}

void Worker::stop()
{
    std::unique_lock<std::recursive_mutex> lock(m_access_mutex);

    signalPumpEnd();

    if (m_runner.joinable()) {
        signalWorkArrived();
        m_runner.join();
    }
}

bool Worker::isPumping()
{
    return m_pumping;
}

void Worker::signalPumpEnd()
{
    m_pumping = false;
}

void Worker::signalWorkArrived()
{
    m_runner_event.set();
}

void Worker::pump()
{
    prePump();

    while(isPumping()) {

        step();

        m_runner_event.waitAndReset(m_sleep_milliseconds);
    }

    postPump();
}

void Worker::step()
{

}

void Worker::prePump()
{

}

void Worker::postPump()
{

}


}

