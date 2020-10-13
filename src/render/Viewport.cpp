#include "Viewport.h"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "../event/EventBus.h"
#include "../event/WindowEvent.h"
#include "../event/MouseEvent.h"
#include "../input/Mouse.h"

Viewport::Viewport(Window* window, float width, float height)
    : m_window(window), m_width(width), m_height(height) {

    calculateMatrices();

    m_eventHandle = EventBus::Subscribe(EVENT_HANDLER(onEvent));
}

void Viewport::calculateProjectionMatrix() {
    m_projection = glm::ortho(0.0f, m_width, 0.0f, m_height, 0.0f, 100.0f);
    m_projectionView = m_projection * m_view;
}

void Viewport::calculateViewMatrix() {
    m_view = glm::mat4(1.0f);
    m_view = glm::translate(m_view, glm::vec3(m_x, m_y, 10.0f));
    m_view = glm::inverse(m_view);
    m_projectionView = m_projection * m_view;
}

void Viewport::calculateMatrices() {
    calculateProjectionMatrix();
    calculateViewMatrix();
}

void Viewport::onEvent(Event *event) {
    // Window resizing
    if (event->getType() == EventType::WindowResize) {
        auto* e = (WindowResizeEvent*) event;
        if (e->getWindow() != m_window) return;

//        m_width = e->getWidth();
//        m_height = e->getHeight();


        float horizontalScale = m_width / m_window->getWidth();
        float verticalScale = m_height / m_window->getHeight();
        std::cout << m_width << " " << m_window->getWidth() << std::endl;

        m_width = e->getWidth() * horizontalScale;
        m_height = e->getHeight() * verticalScale;
        calculateMatrices();
    }

    if (!event->isInCategory(EventCategory::Mouse) || ImGui::GetIO().WantCaptureMouse)
        return;

    // Pan
    if (event->getType() == EventType::MouseMove && Mouse::IsSecondaryDown()) {
        auto* e = (MouseMoveEvent*) event;


        m_x -= e->getDeltaX() * m_width / m_window->getWidth();
        m_y += e->getDeltaY() * m_height / m_window->getHeight();
        calculateViewMatrix();
    }

    // Zoom
    if (event->getType() == EventType::MouseScroll) {
        auto [mouseX, mouseY] = Mouse::GetMousePosition();

        float nextDeltaX = -ZOOM_SENSITIVITY * ((MouseScrollEvent*) event)->getDeltaY();
        float nextScale = m_window->getWidth() / (m_width + nextDeltaX);

        // Calculate and clamp scale, then recalculate deltaX
        float deltaX = (m_window->getWidth() / std::clamp(nextScale, MIN_SCALE, MAX_SCALE)) - m_width;
        float deltaY = (deltaX * (m_window->getHeight() / m_window->getWidth()));

        m_width += deltaX;
        m_x -= mouseX / m_window->getWidth() * deltaX;

        m_height += deltaY;
        m_y -= (m_window->getHeight() - mouseY) / m_window->getHeight() * deltaY;

        calculateMatrices();
    }
}
