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
#include "render/renderer.h"
#include "event/ApplicationEvent.h"
#include "component/HBox.h"
#include "component/StaticRectangle.h"

//todo glfwCharCallback is used for typing in text boxes

bool bad = false;

int win_width = 2000.0f;
int win_height = 2000.0f;

float resolution = 256;

Editor* editor = new Editor(2000, 2000);

void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        bad = !bad;
    if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE)
        editor->viewport.x -= 10;
    if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
        editor->viewport.x += 10;
    if (key == GLFW_KEY_UP && action != GLFW_RELEASE)
        editor->viewport.y += 10;
    if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE)
        editor->viewport.y -= 10;
    if (key == GLFW_KEY_KP_ADD && action != GLFW_RELEASE && resolution < 256)
        resolution += 1;
    if (key == GLFW_KEY_KP_SUBTRACT && action != GLFW_RELEASE && resolution > 5)
        resolution -= 1;
}

float mouse_x = 0.0;
float mouse_y = 0.0;
bool secondary_button_down = false;

std::vector<Component*> stuffToRender;

void mouse_button_callback(GLFWwindow *w, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        secondary_button_down = true;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        secondary_button_down = false;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        auto [x, y] = editor->toCanvasSpace(mouse_x, mouse_y);
        std::cout << "Click: (" << x << " " << y << ")" << std::endl;

        auto* rect = new StaticRectangle(10.0f, 10.0f, RGB(0.3, 0.4, 0.6));
        rect->setX(x / 10 - 5);
        rect->setY(y / 10 - 5);
        stuffToRender.push_back(rect);
    }
}

void mouse_pos_callback(GLFWwindow *w, double x, double y) {
    auto new_x = static_cast<float>(x);
    auto new_y = static_cast<float>(y);

    if (secondary_button_down) {
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

int main() {

    HBox hbox(10.0f);
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





//    auto eventBus = new EventBus();
//    Test test(eventBus);
//
//    WindowCloseEvent testEvent;
//    eventBus->dispatch(&testEvent);












    Renderer renderer;

    window.addEditor(editor);

    // ImGui Init
    float highDPIscaleFactor = 2.0f;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.Fonts->AddFontFromFileTTF("../resources/font/ubuntu.ttf", 18.0f * highDPIscaleFactor, nullptr, nullptr);
    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(highDPIscaleFactor);

    ImGui_ImplGlfw_InitForOpenGL(window.getGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool show_demo_window = false;
    bool show_another_window = false;
    glm::vec4 outerHboxColor(0.2f, 0.3f, 0.8f, 1.0f);

    while (!window.shouldClose()) {
        glClearColor(0.19f, 0.19f, 0.19f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Rendering Setup
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(editor->viewport.x, editor->viewport.y, 10.0f));
        view = glm::inverse(view);

        glm::mat4 proj;
        proj = glm::ortho(0.0f, editor->viewport.width, 0.0f, editor->viewport.height, 0.0f, 100.0f);

        renderer.preRender(proj, view);

        // Coordinate System Test
//        renderer.drawQuad(100.0f, 100.0f);
//        renderer.drawText("This should be on top!", 100.0f, 100.0f, FONT_SIZE_SMALL, glm::vec3(0.5, 0.8f, 0.2f));



        hbox.setBackground(outerHboxColor);
        hbox.layout();
        hbox.render(&renderer);

        for (auto* thing : stuffToRender)
            thing->render(&renderer);
//        rect1.render(&renderer);

        // Quad Rendering
//        renderer.drawQuad(glm::vec2(500.0, 500.0), glm::vec2(1000.0, 1000.0));

        // Font Rendering
//        renderer.drawText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
//        renderer.drawText("Bad", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

        // Test Renderer (Geometry Testing)
//        renderer.drawTest();

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

//            ImGui::ShowDemoWindow();

            ImGui::ColorEdit4("Outer BG Color", glm::value_ptr(outerHboxColor));
            ImGui::End();

        }
//        if (show_demo_window)
//            ImGui::ShowDemoWindow(&show_demo_window);
//        {
//            static int counter = 0;
//
//            int glfw_width = 0, glfw_height = 0, controls_width = 0;
//            glfwGetFramebufferSize(window.getGlfwWindow(), &glfw_width, &glfw_height);
//            controls_width = glfw_width;
//            if ((controls_width /= 3) < 300) { controls_width = 300; }
//
//            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
//
//            ImGui::SetNextWindowSize(
//                    ImVec2(static_cast<float>(controls_width), static_cast<float>(glfw_height - 20)),
//                    ImGuiCond_Always
//            );
//
//            ImGui::SetNextWindowBgAlpha(0.7f);
//            // create a window and append into it
//            ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoResize);
//
//            ImGui::Dummy(ImVec2(0.0f, 1.0f));
//            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Time");
////            ImGui::Text("%s", currentTime(std::chrono::system_clock::now()).c_str());
//
//            ImGui::Dummy(ImVec2(0.0f, 3.0f));
//            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Application");
//            ImGui::Text("Main window width: %d", glfw_width);
//            ImGui::Text("Main window height: %d", glfw_height);
//
//            ImGui::Dummy(ImVec2(0.0f, 3.0f));
//            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "GLFW");
//            ImGui::Text("%s", glfwGetVersionString());
//
//            ImGui::Dummy(ImVec2(0.0f, 10.0f));
//            ImGui::Separator();
//            ImGui::Dummy(ImVec2(0.0f, 10.0f));
//
//            // buttons and most other widgets return true when clicked/edited/activated
//            if (ImGui::Button("Counter button"))
//            {
//                std::cout << "counter button clicked\n";
//                counter++;
//                if (counter == 9) { ImGui::OpenPopup("Easter egg"); }
//            }
//            ImGui::SameLine();
//            ImGui::Text("counter = %d", counter);
//
//            if (ImGui::BeginPopupModal("Easter egg", NULL))
//            {
//                ImGui::Text("Ho-ho, you found me!");
//                if (ImGui::Button("Buy Ultimate Orb")) { ImGui::CloseCurrentPopup(); }
//                ImGui::EndPopup();
//            }
//
//            ImGui::Dummy(ImVec2(0.0f, 15.0f));
//            if (!show_demo_window)
//            {
//                if (ImGui::Button("Open standard demo"))
//                {
//                    show_demo_window = true;
//                }
//            }
//
//            ImGui::Checkbox("show a custom window", &show_another_window);
//            if (show_another_window)
//            {
//                ImGui::SetNextWindowSize(
//                        ImVec2(250.0f, 150.0f),
//                        ImGuiCond_FirstUseEver // after first launch it will use values from imgui.ini
//                );
//                // the window will have a closing button that will clear the bool variable
//                ImGui::Begin("A custom window", &show_another_window);
//
//                ImGui::Dummy(ImVec2(0.0f, 1.0f));
//                ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Some label");
//
//                ImGui::TextColored(ImVec4(128 / 255.0f, 128 / 255.0f, 128 / 255.0f, 1.0f), "%s", "another label");
//                ImGui::Dummy(ImVec2(0.0f, 0.5f));
//
//                ImGui::Dummy(ImVec2(0.0f, 1.0f));
//                if (ImGui::Button("Close"))
//                {
//                    std::cout << "close button clicked\n";
//                    show_another_window = false;
//                }
//
//                ImGui::End();
//            }
//
//            ImGui::End();
//        }

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
