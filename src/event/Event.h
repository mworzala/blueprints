#pragma once

#include <string>

// Create an int with a bit in x position.
#define BIT_ID(x) (1 << x) // NOLINT(hicpp-signed-bitwise)

enum class EventType {
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll,
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

#define EVENT_TYPE(type) static EventType getTypeStatic() { return EventType::type; } \
                         virtual EventType getType() const override { return getTypeStatic(); } \
                         virtual const char* getName() const override { return #type; }

#define EVENT_CATEGORY(category) virtual int getCategories() const override { return category; }
