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

}

void Editor::onMouseButton(int button, int action, int mods) {
    WindowEventReceiver::onMouseButton(button, action, mods);
}

void Editor::onImGuiRender() {

}
