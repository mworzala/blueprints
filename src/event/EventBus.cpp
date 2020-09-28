#include "EventBus.h"

std::map<int, EventHandler> EventBus::m_handlers;

int EventBus::Subscribe(const EventHandler &func) {
    int id = std::rand(); // NOLINT(cert-msc50-cpp)
    m_handlers.insert(std::pair<int, EventHandler>(id, func));
    return id;
}

bool EventBus::UnSubscribe(int id) {
    auto it = m_handlers.find(id);
    if (it != m_handlers.end()) {
        m_handlers.erase(it);
        return true;
    }
    return false;
}

void EventBus::Dispatch(Event *event) {
    for (const auto &item : m_handlers) {
        item.second(event);
    }
}
