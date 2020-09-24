#include "editor.h"

Editor::Editor(float width, float height)
    : m_width(width), m_height(height) {
    viewport = {
            width,
            height,
            -width / 2,
            -height / 2
    };
}

void Editor::render() {

}

void Editor::resize(float new_width, float new_height) {
    float scale = m_width / viewport.width;

    m_width = new_width;
    m_height = new_height;
    viewport.width = new_width / scale;
    viewport.height = new_height / scale;
}

void Editor::onMouseButton(int button, int action, int mods) {
    WindowEventReceiver::onMouseButton(button, action, mods);
}

void Editor::onImGuiRender() {

}

float Editor::getWidth() const {
    return m_width;
}

float Editor::getHeight() const {
    return m_height;
}


