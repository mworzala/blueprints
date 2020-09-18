
#include <chrono>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ImGui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "window.h"
#include "event/ApplicationEvent.h"
#include "component/HBox.h"
#include "component/StaticRectangle.h"
#include "component/Text.h"

#include "render/Renderer.h"

//todo glfwCharCallback is used for typing in text boxes

int win_width = 2000.0f;
int win_height = 2000.0f;

Editor* editor = new Editor(2000, 2000);
HBox hbox(10.0f);

void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;

    if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE)
        editor->viewport.x -= 10;
    if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
        editor->viewport.x += 10;
    if (key == GLFW_KEY_UP && action != GLFW_RELEASE)
        editor->viewport.y += 10;
    if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE)
        editor->viewport.y -= 10;
}

float mouse_x = 0.0;
float mouse_y = 0.0;
bool secondary_button_down = false;

std::vector<Component*> stuffToRender;

void mouse_button_callback(GLFWwindow *w, int button, int action, int mods) {
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        secondary_button_down = true;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        secondary_button_down = false;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        auto [x, y] = editor->toCanvasSpace(mouse_x, mouse_y);
//        std::cout << "Click: (" << x << " " << y << ")" << std::endl;

    if (hbox.contains(x, y))
        hbox.handleClick(x - hbox.getX(), y - hbox.getY());

//        for (int i = 0; i < 1000; i++) {
//            auto* rect = new StaticRectangle(10.0f, 10.0f, RGB(0.3, 0.4, 0.6));
//            rect->setX(x / 10 - 5);
//            rect->setY(y / 10 - 5);
//            stuffToRender.push_back(rect);
//        }
    }
}

void mouse_pos_callback(GLFWwindow *w, double x, double y) {
    auto new_x = static_cast<float>(x);
    auto new_y = static_cast<float>(y);

    if (secondary_button_down && !ImGui::GetIO().WantCaptureMouse) {
        float scale = (float) win_width / editor->viewport.width;
        auto dx = (new_x - mouse_x) / scale;
        auto dy = (new_y - mouse_y) / scale;

        editor->viewport.x -= dx;
        editor->viewport.y += dy;
    }

    mouse_x = new_x;
    mouse_y = new_y;
}

float ZOOM_SENSITIVITY = 20.0f;
float MIN_SCALE = 0.5f;
float MAX_SCALE = 5.0f;

void mouse_scroll_callback(GLFWwindow *w, double dx, double dy) {
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    float nextDeltaX = -ZOOM_SENSITIVITY * static_cast<float>(dy);
    float nextScale = (float) win_width / (editor->viewport.width + nextDeltaX);

    // Calculate and clamp scale, then recalculate deltaX
    float deltaX = ((float) win_width / std::clamp(nextScale, MIN_SCALE, MAX_SCALE)) - editor->viewport.width;
    float deltaY = (deltaX * ((float) win_height / (float) win_width));

    editor->viewport.width += deltaX;
    editor->viewport.x -= mouse_x / (float) win_width * deltaX;

    editor->viewport.height += deltaY;
    editor->viewport.y -= ((float)win_width - mouse_y) / (float) win_height * deltaY;
}

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

Window window("Blueprint Editor (that doesnt do anything)", 2000, 2000,
              key_callback, mouse_button_callback, mouse_pos_callback, mouse_scroll_callback);

StaticRectangle rect7(30.0f, 50.0f, RGB(0.2, 0.1, 0.4));

int main() {

    Renderer::Init();
    hbox.setBackground(RGB(1.0, 0.9, 0.8));
    StaticRectangle rect1(50.0f, 20.0f, RGB(0.9, 0.9, 0.5));
    StaticRectangle rect2(10.0f, 100.0f, RGB(0.6, 0.8, 0.1));

    HBox hbox2(5.0f);
    hbox2.setBackground(RGB(0.1, 0.2, 0.3));
    hbox2.setPadding(10.0f, 10.0f, 0.0f, 00.0f);
    StaticRectangle rect3(10.0f, 10.0f, RGB(0.2, 0.4, 0.1));
    StaticRectangle rect4(30.0f, 50.0f, RGB(0.7, 0.1, 0.4));
    rect4.setMargin(5.0f);

    hbox.addChild(&rect1);
    hbox.addChild(&rect2);
    hbox.addChild(&hbox2);

    hbox2.addChild(&rect3);
    hbox2.addChild(&rect4);

//    hbox.setBackground(RGB(1.0, 0.9, 0.8));
//    StaticRectangle rect4(30.0f, 50.0f, RGB(0.7, 0.1, 0.4));
//    rect4.setMargin(5.0f);
//    hbox.addChild(&rect4);
//    StaticRectangle rect5(30.0f, 50.0f, RGB(0.7, 0.1, 0.4));
//    rect5.setMargin(5.0f);
//    hbox.addChild(&rect5);
//
//    HBox hboxAlt(6.9);
//    hboxAlt.setBackground(RGB(0.2, 0.5, 0.8));
//    StaticRectangle rect6(30.0f, 50.0f, RGB(0.2, 0.1, 0.4));
//    rect6.setMargin(5.0f);
//    hboxAlt.addChild(&rect6);
//
//    HBox hboxAlt2(6.9);
//    hboxAlt2.setBackground(RGB(0.2, 0.5, 0.8));
//    rect7.setMargin(5.0f);
//    hboxAlt2.addChild(&rect7);
//
//    hbox.addChild(&hboxAlt);
//    hbox.addChild(&hboxAlt2);

    StaticRectangle a(100, 100, RGB(0.7, 0.1, 0.4));

    Text t("Hello, World");
    t.setBackground(RGB(0.6, 0.7, 0.8));



//    hbox.setOnClick([](float x, float y) {
//        std::cout << "Clicked on HBox!" << std::endl;
//    });
//    hboxAlt.setOnClick([](float x, float y) {
//        std::cout << "Clicked on HBoxAlt!" << std::endl;
//    });
//    hboxAlt2.setOnClick([](float x, float y) {
//        std::cout << "Clicked on HBoxAlt!" << std::endl;
//    });
//
//    rect4.setOnClick([](float x, float y) {
//        std::cout << "Clicked on rect 4!" << std::endl;
//    });
//    rect5.setOnClick([](float x, float y) {
//        std::cout << "Clicked on rect 5!" << std::endl;
//    });
//    rect6.setOnClick([](float x, float y) {
//        std::cout << "Clicked on rect 6!" << std::endl;
//    });
//    rect7.setOnClick([](float x, float y) {
//        std::cout << "Clicked on rect 6!" << std::endl;
//        rect7.setBackground(RGB(0.4, 0.7, 0.2));
//    });



//    auto eventBus = new EventBus();
//    Test test(eventBus);
//
//    WindowCloseEvent testEvent;
//    eventBus->dispatch(&testEvent);

    window.addEditor(editor);

    // ImGui Init
    float highDPIscaleFactor = 2.0f;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.Fonts->AddFontFromFileTTF("../resources/font/ubuntu.ttf", 18.0f * highDPIscaleFactor, nullptr, nullptr);
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(highDPIscaleFactor);

    ImGui_ImplGlfw_InitForOpenGL(window.getGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glm::vec4 outerHboxColor(0.2f, 0.3f, 0.8f, 1.0f);

    auto last_time = std::chrono::steady_clock::now();

    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!window.shouldClose()) {
        auto now_time = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> delta = now_time - last_time;
        auto deltaCount = delta.count();
        float fps = 1000.0f / (float) deltaCount;
        last_time = now_time;

        glClearColor(0.19f, 0.19f, 0.19f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Rendering Setup
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(editor->viewport.x, editor->viewport.y, 10.0f));
        view = glm::inverse(view);

        glm::mat4 proj;
        proj = glm::ortho(0.0f, editor->viewport.width, 0.0f, editor->viewport.height, 0.0f, 100.0f);

        Renderer::NewFrame(proj, view);

        hbox.setBackground(outerHboxColor);
        hbox.layout();
//        hbox.render();
//        a.render();
        t.render();

        for (auto* thing : stuffToRender)
            thing->render();


        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Test Window");

            ImGui::Text("ImGui Test");
            ImGui::Text("This needs to be done very much better");
            ImGui::Text("Also I need to organize all of my rendering");
            ImGui::Text("Because it is a big mess :(");

            ImGui::ColorEdit4("Outer BG Color", glm::value_ptr(outerHboxColor));

            ImGui::End();

        }

//        ImGui::ShowDemoWindow();

        {
            ImGui::Begin("Debug");

            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Draw Calls: %d", Renderer::GetDrawCalls());

            if (ImGui::Button("Clear")) {
                for (auto* thing : stuffToRender)
                    delete (StaticRectangle*) thing;
                stuffToRender.clear();
            }

            ImGui::End();
        }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBuffers();
    }

    for (auto* thing : stuffToRender)
        delete (StaticRectangle*) thing;

    delete editor;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
