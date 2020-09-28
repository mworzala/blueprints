#include "Editor.h"

#include "event/EventBus.h"
#include "event/WindowEvent.h"

Editor::Editor(Window* window, float width, float height)
    : m_window(window), m_width(width), m_height(height),
      m_viewport(window, width, height) {
    m_eventHandle = EventBus::Subscribe(EVENT_HANDLER(onEvent));
}

Editor::~Editor() {
    EventBus::UnSubscribe(m_eventHandle);
}

void Editor::render() {

}

void Editor::onImGuiRender() {

}

void Editor::onEvent(Event *event) {
//    if (event->getType() == EventType::WindowResize) {
//        auto* e = (WindowResizeEvent*) event;
//        if (e->getWindow() != m_window) return;
//
//        onResize(e->getWidth(), e->getHeight());
//    }
}

void Editor::onResize(float new_width, float new_height) {
//    float scale = m_width / viewport.width;
//
//    m_width = new_width;
//    m_height = new_height;
//    viewport.width = new_width / scale;
//    viewport.height = new_height / scale;
}


