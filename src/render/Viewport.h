#pragma once

#include <glm/glm.hpp>

#include "../Window.h"
#include "../event/Event.h"

const float ZOOM_SENSITIVITY = 20.0f;
const float MIN_SCALE = 0.5f;
const float MAX_SCALE = 5.0f;

class Viewport {
private:
    Window* m_window;

    float m_x = 0, m_y = 0;
    float m_width, m_height;

    int m_eventHandle;

    glm::mat4 m_projection{};
    glm::mat4 m_view{};
    glm::mat4 m_projectionView{};

public:
    Viewport(Window* window, float width, float height);
    ~Viewport() = default;

    float getX() const { return m_x; }
    float getY() const { return m_y; }

    float getWidth() const { return m_width; }
    float getHeight() const { return m_height; }

    const glm::mat4& getProjectionViewMatrix() const { return m_projectionView; }

private:
    void calculateProjectionMatrix();
    void calculateViewMatrix();
    void calculateMatrices();

    void onEvent(Event* event);
};


