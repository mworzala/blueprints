#include "editor.h"

Editor::Editor(Window *window)
    : m_window(window), m_width(window->getWidth()), m_height(window->getWidth()) {
    m_viewport = {
            .width = window->getWidth(),
            .height = window->getHeight(),
            .x = 0.0f,
            .y = 0.0f
    };
}

void Editor::resize(float new_width, float new_height) {

}
