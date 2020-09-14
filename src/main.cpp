#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "window.h"
#include "render/renderer.h"
#include "event/ApplicationEvent.h"
#include "component/HBox.h"
#include "component/StaticRectangle.h"

//todo glfwCharCallback is used for typing in text boxes

bool bad = false;

int win_width = 2000.0f;
int win_height = 2000.0f;

float resolution = 256;


Viewport viewport;

void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        bad = !bad;
    if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE)
        viewport.x -= 10;
    if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
        viewport.x += 10;
    if (key == GLFW_KEY_UP && action != GLFW_RELEASE)
        viewport.y += 10;
    if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE)
        viewport.y -= 10;
    if (key == GLFW_KEY_KP_ADD && action != GLFW_RELEASE && resolution < 256)
        resolution += 1;
    if (key == GLFW_KEY_KP_SUBTRACT && action != GLFW_RELEASE && resolution > 5)
        resolution -= 1;
}

float mouse_x = 0.0;
float mouse_y = 0.0;
bool secondary_button_down = false;

void mouse_button_callback(GLFWwindow *w, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        secondary_button_down = true;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        secondary_button_down = false;
}

void mouse_pos_callback(GLFWwindow *w, double x, double y) {
    auto new_x = static_cast<float>(x);
    auto new_y = static_cast<float>(y);

    if (secondary_button_down) {
        float scale = (float) win_width / viewport.width;
        auto dx = (new_x - mouse_x) / scale;
        auto dy = (new_y - mouse_y) / scale;

        viewport.x -= dx;
        viewport.y += dy;
    }

    mouse_x = new_x;
    mouse_y = new_y;
}

float ZOOM_SENSITIVITY = 20.0f;
float MIN_SCALE = 0.5f;
float MAX_SCALE = 5.0f;

void mouse_scroll_callback(GLFWwindow *w, double dx, double dy) {
    float nextDeltaX = -ZOOM_SENSITIVITY * static_cast<float>(dy);
    float nextScale = (float) win_width / (viewport.width + nextDeltaX);

    // Calculate and clamp scale, then recalculate deltaX
    float deltaX = ((float) win_width / std::clamp(nextScale, MIN_SCALE, MAX_SCALE)) - viewport.width;
    float deltaY = (deltaX * ((float) win_height / (float) win_width));

    viewport.width += deltaX;
    viewport.x -= mouse_x / (float) win_width * deltaX;

    viewport.height += deltaY;
    viewport.y -= ((float)win_width - mouse_y) / (float) win_height * deltaY;
}

Window window("Blueprint Editor (that doesnt do anything)", 2000, 2000,
              key_callback, mouse_button_callback, mouse_pos_callback, mouse_scroll_callback);

Editor* editor = new Editor(2000, 2000);

class Test {
private:
    const char* m_name = "TestEventHandler";

public:
    explicit Test(EventBus* eventBus) {
        eventBus->subscribe(EventType::WindowClose, EVENT_HANDLER(HandleEvent));
    }

    void HandleEvent(Event* event) {
        std::cout << m_name << " <> " << event->toString() << std::endl;
    }
};

int main() {

    HBox hbox(10.0f);
    StaticRectangle rect1(50.0f, 20.0f);
    StaticRectangle rect2(10.0f, 100.0f);

    HBox hbox2(5.0f);
    StaticRectangle rect3(10.0f, 10.0f);
    StaticRectangle rect4(30.0f, 50.0f);


    hbox.addChild(&rect1);
    hbox.addChild(&rect2);
    hbox.addChild(&hbox2);
    std::cout << "Hi" << std::endl;
    hbox2.addChild(&rect3);
    hbox2.addChild(&rect4);






//    auto eventBus = new EventBus();
//    Test test(eventBus);
//
//    WindowCloseEvent testEvent;
//    eventBus->dispatch(&testEvent);












    Renderer renderer;

    window.addEditor(editor);

    while (!window.shouldClose()) {
        glClearColor(0.19f, 0.19f, 0.19f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Rendering Setup
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(viewport.x, viewport.y, 10.0f));
        view = glm::inverse(view);

        glm::mat4 proj;
        proj = glm::ortho(0.0f, viewport.width, 0.0f, viewport.height, 0.0f, 100.0f);

        renderer.preRender(proj, view);

        // Coordinate System Test
//        renderer.drawQuad(100.0f, 100.0f);
//        renderer.drawText("This should be on top!", 100.0f, 100.0f, FONT_SIZE_SMALL, glm::vec3(0.5, 0.8f, 0.2f));

        hbox.layout();
        hbox.render(&renderer);
//        rect1.render(&renderer);

        // Quad Rendering
//        renderer.drawQuad(glm::vec2(500.0, 500.0), glm::vec2(1000.0, 1000.0));

        // Font Rendering
//        renderer.drawText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
//        renderer.drawText("Bad", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

        // Test Renderer (Geometry Testing)
//        renderer.drawTest();

        window.swapBuffers();
    }

    delete editor;

    glfwTerminate();
    return 0;
}
