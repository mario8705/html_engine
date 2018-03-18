#pragma once
#include <vector>
#include <functional>
#include <string>

class EventEmitter
{
public:
    EventEmitter();

    int On(std::string eventName, std::function<void()> callback);
    void Off(int eventId);
    
    void Emit(std::string eventName);

private:
    struct EventHandler
    {
        int id;
        std::string eventName;
        std::function<void()> callback;
    };

    int m_lastId;
    std::vector<EventHandler> m_eventHandlers;
};
