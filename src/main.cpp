
#include <chrono>
#include <iostream>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ImGui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "input/Mouse.h"
#include "event/MouseEvent.h"
#include "Editor.h"
#include "Window.h"
#include "event/WindowEvent.h"
#include "component/HBox.h"
#include "component/StaticRectangle.h"
#include "component/Text.h"

#include "render/Renderer.h"
#include "event/EventBus.h"

//todo glfwCharCallback is used for typing in text boxes

float scaling = 2.0f;

float initialWidth = 1280.0f * scaling;
float initialHeight = 720.0f * scaling;

Window* window = new Window("Blueprint Editor (that doesnt do anything)", static_cast<int>(initialWidth), static_cast<int>(initialHeight));
Editor* editor = new Editor(window, initialWidth, initialHeight);
HBox hbox(10.0f, HBoxAlignment::TOP);

void TempUiControls(Event* event) {
    if (event->getType() == EventType::WindowResize)
        std::cout << event->toString() << std::endl;

    if (event->isInCategory(EventCategory::Mouse) && !ImGui::GetIO().WantCaptureMouse) {
        // HBox clicking
        if (event->getType() == EventType::MouseButtonRelease) {
            if (((MouseButtonReleaseEvent*) event)->getButton() == 0) {
                auto [mouseX, mouseY] = Mouse::GetMousePosition();
                auto [x, y] = editor->toCanvasSpace(mouseX, mouseY);

                if (hbox.contains(x, y))
                    hbox.handleClick(x - hbox.getX(), y - hbox.getY());
            }
        }
    }
}

//void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
//    if (ImGui::GetIO().WantCaptureKeyboard)
//        return;
//
//    if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE)
//        editor->viewport.x -= 10;
//    if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
//        editor->viewport.x += 10;
//    if (key == GLFW_KEY_UP && action != GLFW_RELEASE)
//        editor->viewport.y += 10;
//    if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE)
//        editor->viewport.y -= 10;
//
//    if (key == GLFW_KEY_G)
//        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
//}



StaticRectangle rect7(30.0f, 50.0f, RGB(0.2, 0.1, 0.4));

int main() {
    Mouse::Init();

    EventBus::Subscribe(TempUiControls);

    Renderer::Init();
    hbox.setBackground(RGB(1.0, 0.9, 0.8));
    StaticRectangle rect1(50.0f, 20.0f, RGB(0.9, 0.9, 0.5));
    rect1.setMargin(2);
    StaticRectangle rect2(10.0f, 100.0f, RGB(0.6, 0.8, 0.1));

    HBox hbox2(5.0f, HBoxAlignment::TOP);
    hbox2.setBackground(RGB(0.1, 0.2, 0.3));
    hbox2.setPadding(10.0f, 10.0f, 0.0f, 00.0f);
    hbox2.setMargin(10);
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
    t.setPadding(10);
    t.setMargin(10);
    t.setBackground(RGB(0.6, 0.7, 0.8));

    hbox.setOnClick([](float x, float y) {
        std::cout << "Clicked on HBox!" << std::endl;
    });

    // ImGui Init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontFromFileTTF("../resources/font/ubuntu.ttf", 18.0f * scaling, nullptr, nullptr);
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(scaling);

    ImGui_ImplGlfw_InitForOpenGL(window->getGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glm::vec4 outerHboxColor(0.2f, 0.3f, 0.8f, 1.0f);

    auto last_time = std::chrono::steady_clock::now();

    glfwSwapInterval(0.5);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    hbox.addChild(&t);

    Shader shader("../resources/shader/polygon.vert", "../resources/shader/polygon.frag", "../resources/shader/polygon.geom");
    VertexArray polyVao;
    auto* polyPosVbo = new VertexBuffer();
    float pos[] = { 0.0, 0.0 };
    polyPosVbo->setData(pos, sizeof(pos));
    auto* polyOptsVbo = new VertexBuffer(4);
    // vertices, filled, radius, thickness
    float opts[] = { 7, 0, 0.5, 0.1 };
    polyOptsVbo->setData(opts, sizeof(opts));
    auto* polyColourVbo = new VertexBuffer(3);
    float colour[] = { 0.2, 0.2, 0.7 };
    polyColourVbo->setData(colour, sizeof(colour));

    polyVao.addVertexBuffer(polyPosVbo);
    polyVao.addVertexBuffer(polyOptsVbo);
    polyVao.addVertexBuffer(polyColourVbo);

    while (!window->shouldClose()) {
        auto now_time = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> delta = now_time - last_time;
        auto deltaCount = delta.count();
        float fps = 1000.0f / (float) deltaCount;
        last_time = now_time;

        glClearColor(0.19f, 0.19f, 0.19f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Rendering Setup
//        glm::mat4 view = glm::mat4(1.0f);
//        view = glm::translate(view, glm::vec3(editor->viewport.x, editor->viewport.y, 10.0f));
//        view = glm::inverse(view);

//        glm::mat4 proj;
//        proj = glm::ortho(0.0f, editor->viewport.width, 0.0f, editor->viewport.height, 0.0f, 100.0f);

        Renderer::NewFrame(editor->getViewport()->getProjectionViewMatrix());
//        Renderer::NewFrame(proj, view);

        hbox.setBackground(outerHboxColor);
        hbox.layout();
        hbox.render();


        shader.use();
        polyVao.bind();
        glDrawArrays(GL_POINTS, 0, 1);
        polyVao.unbind();

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool demoWindowOpen = false;
        if (demoWindowOpen)
            ImGui::ShowDemoWindow(&demoWindowOpen);

        {
            ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_DockNodeHost);

            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Draw Calls: %d", Renderer::GetDrawCalls());

            if (ImGui::Button("Open Demo Window"))
                demoWindowOpen = true;

            ImGui::End();
        }

        {
            ImGui::Begin("Test Window", nullptr,ImGuiWindowFlags_DockNodeHost);

            ImGui::Text("ImGui Test");
            ImGui::ColorEdit4("Outer BG Color", glm::value_ptr(outerHboxColor));

            ImGui::End();
        }

        {
            ImGui::Begin("DT Window 1");

            ImGui::TextUnformatted("Docking Test 1");

            ImGui::End();
        }

        {
            ImGui::Begin("DT Window 2");

            ImGui::TextUnformatted("Docking Test 2");

            ImGui::End();
        }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window->swapBuffers();
    }

    delete editor;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
