#include "Mouse.h"

#include "../event/MouseEvent.h"

std::set<int> Mouse::m_buttonsDown;
float Mouse::m_x;
float Mouse::m_y;

void Mouse::Init() {
    EventBus::Subscribe(OnEvent);
}

bool Mouse::IsPrimaryDown() {
    return IsButtonDown(0);
}

bool Mouse::IsSecondaryDown() {
    return IsButtonDown(1);
}

bool Mouse::IsButtonDown(int button) {
    return m_buttonsDown.find(button) != m_buttonsDown.end();
}

void Mouse::OnEvent(Event *event) {
    if (!event->isInCategory(EventCategory::Mouse))
        return;

    // Button down
    if (event->getType() == EventType::MouseButtonPress)
        m_buttonsDown.insert(((MouseButtonReleaseEvent*) event)->getButton());

    // Button up
    if (event->getType() == EventType::MouseButtonRelease) {
        auto it = m_buttonsDown.find(((MouseButtonReleaseEvent*) event)->getButton());
        if (it != m_buttonsDown.end())
            m_buttonsDown.erase(it);
    }

    // Mouse move
    if (event->getType() == EventType::MouseMove) {
        auto* e = (MouseMoveEvent*) event;
        m_x = e->getX();
        m_y = e->getY();
    }
}

std::tuple<float, float> Mouse::GetMousePosition() {
    return { m_x, m_y };
}
