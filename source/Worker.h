#pragma once

#include "Project.h"

namespace re {

class Worker {
public:
    Worker();
    virtual ~Worker();

    void setTag(std::string TAG);
    void setSleepMs(int millis);

    void start();
    void stop();
    bool isPumping();

    void signalPumpEnd();

    void signalWorkArrived();

protected:
    std::string TAG;

    // override these methods to add work on the thread
    virtual void step();
    virtual void prePump();
    virtual void postPump();

private:
    std::recursive_mutex m_access_mutex;
    std::thread m_runner;
    WaitEvent m_runner_event;
    int m_sleep_milliseconds;
    bool m_pumping;

    virtual void pump();
};

}


