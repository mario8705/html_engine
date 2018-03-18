#include "EventEmitter.h"

using namespace std;

EventEmitter::EventEmitter()
    : m_lastId(1)
{
}

int EventEmitter::On(string name, function<void()> callback)
{
    EventHandler eventHandler;
    eventHandler.eventName = name;
    eventHandler.callback = callback;
    eventHandler.id = m_lastId++;

    m_eventHandlers.push_back(eventHandler);

    return eventHandler.id;
}

void EventEmitter::Off(int eventId)
{
    auto it = m_eventHandlers.begin();

    while (it != m_eventHandlers.end())
    {
        if ((*it).id == eventId)
        {
            m_eventHandlers.erase(it);
            break;
        }
    }
}

void EventEmitter::Emit(string name)
{
    for (auto it = m_eventHandlers.begin(); it != m_eventHandlers.end(); ++it)
    {
        if ((*it).eventName == name)
        {
            (*it).callback();
        }
    }
}