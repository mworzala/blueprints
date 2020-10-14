
#include <chrono>
#include <iostream>
#include <algorithm>
#include <random>
#include <time.h>

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
#include "render/instanced/InstancedRenderer.h"

#include "render/font/GuillotinePack.h"

//todo glfwCharCallback is used for typing in text boxes

float scaling = 1.0f;

float initialWidth = 1280.0f * scaling;
float initialHeight = 720.0f * scaling;

Window *window = new Window("Blueprint Editor (that doesnt do anything)", static_cast<int>(initialWidth),
                            static_cast<int>(initialHeight), scaling);
Editor *editor = new Editor(window, initialWidth, initialHeight);

int sortIds(std::pair<int, int> a, std::pair<int, int> b) {
    return a.second > b.second;
}

void TempUiControls(Event *event) {
    if (event->getType() == EventType::WindowResize)
        std::cout << event->toString() << std::endl;

    if (event->isInCategory(EventCategory::Mouse) && !ImGui::GetIO().WantCaptureMouse) {
        // HBox clicking
        if (event->getType() == EventType::MouseButtonRelease) {
            if (((MouseButtonReleaseEvent *) event)->getButton() == 0) {
                auto[mouseX, mouseY] = Mouse::GetMousePosition();
                auto[x, y] = editor->toCanvasSpace(mouseX, mouseY);

//                if (hbox.contains(x, y))
//                    hbox.handleClick(x - hbox.getX(), y - hbox.getY());
            }
        }
    }
}

int main() {

    Mouse::Init();

    EventBus::Subscribe(TempUiControls);

    srand(time(nullptr));

    Renderer::Init();

    // ImGui Init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontFromFileTTF("../resources/font/ubuntu.ttf", 18.0f * scaling, nullptr, nullptr);
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(scaling);

    ImGui_ImplGlfw_InitForOpenGL(window->getGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");


    auto last_time = std::chrono::steady_clock::now();

    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    // NON INSTANCED POLYGON RENDERING
//    Shader shader("../resources/shader/polygon.vert", "../resources/shader/polygon.frag", "../resources/shader/polygon.geom");
//    VertexArray polyVao;
//    auto* polyPosVbo = new VertexBuffer();
//    float pos[] = { 0.0, 0.0 };
//    polyPosVbo->setData(pos, sizeof(pos));
//    auto* polyOptsVbo = new VertexBuffer(4);
//    // vertices, filled, radius, thickness
//    float opts[] = { 7, 0, 0.5, 0.1 };
//    polyOptsVbo->setData(opts, sizeof(opts));
//    auto* polyColourVbo = new VertexBuffer(3);
//    float colour[] = { 0.2, 0.2, 0.7 };
//    polyColourVbo->setData(colour, sizeof(colour));
//
//    polyVao.addVertexBuffer(polyPosVbo);
//    polyVao.addVertexBuffer(polyOptsVbo);
//    polyVao.addVertexBuffer(polyColourVbo);

    // INSTANCED QUAD RENDERING
//    InstancedRenderer::Init();
//    auto* q1 = InstancedRenderer::AllocateQuad();
//    auto* q2 = InstancedRenderer::AllocateQuad();
//    auto* q3 = InstancedRenderer::AllocateQuad();
//
//    InstancedQuadRenderer::PreUpdate();
//
//    q1->translate(0.5, 0, -1);
//    q1->setColour(RGB(0.3, 0.7, 0.2));
//    q1->setScale(2000, 1000);
//    q1->update();
//
//    q2->translate(0, 0, 0);
//    q2->setColour(RGB(0.2, 0.3, 0.7));
//    q2->setScale(1000, 1000);
//    q2->update();
//
//    float q3x = 500;
//    q3->translate(q3x, 0, 1);
//    q3->setColour(RGB(0.9, 0.1, 0.5));
//    q3->setScale(500, 500);
//    q3->update();
//
//    InstancedQuadRenderer::PostUpdate();

    // FONT TESTING
    Shader fontShader("../resources/shader/instanced/font.vert", "../resources/shader/instanced/font.frag");
    auto *fontVao = new VertexArray();

    float vertices[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,

            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };
    auto *vertBuffer = new VertexBuffer(2);
    vertBuffer->setData(vertices, sizeof(vertices));
    fontVao->addVertexBuffer(vertBuffer);

    float data[] = {
            1000.0, 1000.0,
            1000.0, 1000.0,
            1000.0, 1000.0,

            1000.0, 1000.0,
            1000.0, 1000.0,
            1000.0, 1000.0,
    };
    auto *dataBuffer = new VertexBuffer();
    dataBuffer->setData(data, sizeof(data));
    fontVao->addVertexBuffer(dataBuffer);

    FT_Library ft;
    FT_Face face;
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
    if (FT_New_Face(ft, "../resources/font/ubuntu.ttf", 0, &face))
        throw std::runtime_error("ERROR::FREETYPE: Failed to load font");

    FT_Set_Pixel_Sizes(face, 0, 96);

    unsigned int area = 0;
    std::vector<std::pair<int, int>> ids;
    for (int i = 32; i < 127; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            std::cout << "Failed to load character " << (char) i << "." << std::endl;
            continue;
        }

        unsigned int a = (face->glyph->bitmap.width * face->glyph->bitmap.rows);
        std::pair<int, int> id(i, (int) face->glyph->bitmap.width);  // width
        ids.push_back(id);
        area += a;
    }

    std::sort(ids.begin(), ids.end(), sortIds);

    int atlasWidth = std::sqrt(area);
//    atlasWidth *= 1.06;  // With one channel
//    atlasWidth *= 0.58;  // With no retry
    atlasWidth *= 0.535; // With no specified order
    int atlasHeight = atlasWidth;

    unsigned int ubuntuAtlasId;
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &ubuntuAtlasId);
    glBindTexture(GL_TEXTURE_2D, ubuntuAtlasId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GuillotinePack::StartPack(4, atlasWidth, atlasHeight);

    for (auto p : ids) {
        std::cout << p.first << std::endl;
        if (FT_Load_Char(face, p.first, FT_LOAD_RENDER))
            continue;

        int gw = (int) face->glyph->bitmap.width;
        int gh = (int) face->glyph->bitmap.rows;

        bool inserted = GuillotinePack::PackData_t(gw, gh, face->glyph->bitmap.buffer);
        if (!inserted)
            throw std::runtime_error("Failed to pack texture with the given size!");
    }

    unsigned char* image = GuillotinePack::EndPack();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete[] image;

    while (!window->shouldClose()) {
        auto now_time = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> delta = now_time - last_time;
        auto deltaCount = delta.count();
        float fps = 1000.0f / (float) deltaCount;
        last_time = now_time;

        glClearColor(0.19f, 0.19f, 0.19f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


//        InstancedRenderer::Render(editor->getViewport()->getProjectionViewMatrix());


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ubuntuAtlasId);
        fontShader.use();
        fontShader.setMat4("projection_view", editor->getViewport()->getProjectionViewMatrix());
        fontShader.setInt("font_tex", 0);
        static int ch = 0;
        fontShader.setInt("channel", ch);
        fontVao->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);


//        Renderer::NewFrame(editor->getViewport()->getProjectionViewMatrix());

//        shader.use();
//        polyVao.bind();
//        glDrawArrays(GL_POINTS, 0, 1);
//        polyVao.unbind();


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
            ImGui::Text("Draw Calls: %d", Renderer::GetDrawCalls() + InstancedRenderer::GetDrawCalls());
//            ImGui::Text("Quads: %d", quads);

            if (ImGui::Button("Open Demo Window"))
                demoWindowOpen = true;

//            if (ImGui::SliderFloat("Q3 X Position", &q3x, 0.0, 1000.0)) {
//                InstancedQuadRenderer::PreUpdate();
//                q3->translate(q3x, 0, 1);
//                InstancedQuadRenderer::PostUpdate();
//            }

            ImGui::End();
        }

        {
            ImGui::Begin("Font Texture Atlas");
            ImGui::Image((void *) (GLuint) ubuntuAtlasId, ImVec2((float) atlasWidth * 1.5, (float) atlasHeight * 1.5));

            ImGui::SliderInt("Colour Channel", &ch, 0, 3);
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
