#pragma once

#include <functional>
#include <map>

#include "Event.h"

typedef std::function<void (Event*)> EventHandler;
#define EVENT_HANDLER(func) [this](Event* e) { return func(e); }

class EventBus {
private:
    static std::map<int, EventHandler> m_handlers;

public:
    static int Subscribe(const EventHandler& func);

    static bool UnSubscribe(int id);

    static void Dispatch(Event* event);
};
