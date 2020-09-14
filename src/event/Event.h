#pragma once

#include <ostream>
#include <string>
#include <functional>
#include <vector>

// Create an int with a bit in x position.
#define BIT_ID(x) (1 << x) // NOLINT(hicpp-signed-bitwise)

enum class EventType {
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory {
    None = 0,
    Application = BIT_ID(0),
    Input = BIT_ID(1),
    Keyboard = BIT_ID(2),
    Mouse = BIT_ID(3),
    MouseButton = BIT_ID(4),
};

class Event {
public:
    virtual ~Event() = default;

    virtual EventType getType() const = 0;
    virtual const char* getName() const = 0;
    virtual int getCategories() const = 0;
    virtual std::string toString() const { return getName(); }

    bool isInCategory(EventCategory category) const {
        return getCategories() & category;
    }
};

//inline std::ostream& operator <<(std::ostream os, Event& e) {
//    return os << e.toString();
//}

#define EVENT_TYPE(type) static EventType getTypeStatic() { return EventType::type; } \
                         virtual EventType getType() const override { return getTypeStatic(); } \
                         virtual const char* getName() const override { return #type; }

#define EVENT_CATEGORY(category) virtual int getCategories() const override { return category; }

class EventBus {
private:
    std::vector<std::function<void (Event*)>> m_handlers;
    
public:
    EventBus() = default;
    ~EventBus() = default;

    void subscribe(EventType type, const std::function<void (Event*)>& func) {
        m_handlers.push_back(func);
    }

    void dispatch(Event* event) {
        for (const auto& handler : m_handlers)
            handler(event);
    }
};

#define EVENT_HANDLER(func) [this](Event* e) { return func(e); }