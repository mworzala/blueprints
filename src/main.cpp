
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

//todo glfwCharCallback is used for typing in text boxes

float scaling = 2.0f;

float initialWidth = 1280.0f * scaling;
float initialHeight = 720.0f * scaling;

Window *window = new Window("Blueprint Editor (that doesnt do anything)", static_cast<int>(initialWidth),
                            static_cast<int>(initialHeight));
Editor *editor = new Editor(window, initialWidth, initialHeight);
HBox hbox(10.0f, HBoxAlignment::TOP);

struct Rectangle {
    int x;
    int y;
    int width;
    int height;
};

#define CHANNELS 4

std::vector<Rectangle> freeRectangles[CHANNELS];

int area(Rectangle src) {
    return src.width * src.height;
}

bool fits(Rectangle src, int width, int height) {
    return src.width >= width && src.height >= height;
}

std::tuple<Rectangle, Rectangle> insert(Rectangle src, int width, int height) {
    if (width > height) {
        // Split horizontally
        return {
                {src.x + width, src.y,              src.width - width, /**/height},        // Right
                {src.x,         src.y + height, /**/src.width,             src.height - height}   // Bottom
        };
    } else {
        // Split vertically
        return {
                {src.x,         src.y + height, /**/width,                 src.height - height},  // Bottom
                {src.x + width, src.y,              src.width - width, /**/src.height} // Right
        };
    }
}

int sortIds(std::pair<int, int> a, std::pair<int, int> b) {
    return a.second > b.second;
}

int findBestCut(int j, int width, int height) {
    int best = -1;
    int bestExtra = 2147483647;
    for (int i = 0; i < freeRectangles[j].size(); i++) {
        if (!fits(freeRectangles[j][i], width, height)) continue;

        auto[sm, lg] = insert(freeRectangles[j][i], width, height);
        auto smArea = area(sm);
        if (smArea < bestExtra) {
            bestExtra = smArea;
            best = i;
        }
    }
    return best;
}


std::tuple<int, Rectangle> tryInsertAny(int w, int h) {
    for (int i = 0; i < CHANNELS; i++) {
        int bestFit = findBestCut(i, w, h);
        if (bestFit == -1)
            continue;

        auto r = freeRectangles[i][bestFit];
        freeRectangles[i].erase(freeRectangles[i].begin() + bestFit);
        auto[sm, lg] = insert(r, w, h);
        freeRectangles[i].push_back(sm);
        freeRectangles[i].push_back(lg);
        return { i, r };
    }
    throw std::runtime_error("Cannot pack texture in the given size!");
}

void myTexSubImage2D(unsigned char* image, int imageWidth, int channel, int x, int y, int w, int h, const unsigned char* data) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image[(((y + i) * imageWidth) + (x + j)) * CHANNELS + channel] = data[(i * w) + j];
        }
    }
}

void MessageCallback(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar *message,
                     const void *userParam) {
//    std::cout << GL_DEBUG_SEVERITY_HIGH << " " << GL_DEBUG_SEVERITY_MEDIUM << " " << GL_DEBUG_SEVERITY_LOW << " " << GL_DEBUG_SEVERITY_NOTIFICATION << std::endl;

//    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
//             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
//             type, severity, message );
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

    srand(time(nullptr));

    Renderer::Init();
//    hbox.setBackground(RGB(1.0, 0.9, 0.8));
//    StaticRectangle rect1(50.0f, 20.0f, RGB(0.9, 0.9, 0.5));
//    rect1.setMargin(2);
//    StaticRectangle rect2(10.0f, 100.0f, RGB(0.6, 0.8, 0.1));
//
//    HBox hbox2(5.0f, HBoxAlignment::TOP);
//    hbox2.setBackground(RGB(0.1, 0.2, 0.3));
//    hbox2.setPadding(10.0f, 10.0f, 0.0f, 00.0f);
//    hbox2.setMargin(10);
//    StaticRectangle rect3(10.0f, 10.0f, RGB(0.2, 0.4, 0.1));
//    StaticRectangle rect4(30.0f, 50.0f, RGB(0.7, 0.1, 0.4));
//    rect4.setMargin(5.0f);
//
//    hbox.addChild(&rect1);
//    hbox.addChild(&rect2);
//    hbox.addChild(&hbox2);
//
//    hbox2.addChild(&rect3);
//    hbox2.addChild(&rect4);

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

//    StaticRectangle a(100, 100, RGB(0.7, 0.1, 0.4));
//
//    Text t("Hello, World");
//    t.setPadding(10);
//    t.setMargin(10);
//    t.setBackground(RGB(0.6, 0.7, 0.8));
//
//    hbox.setOnClick([](float x, float y) {
//        std::cout << "Clicked on HBox!" << std::endl;
//    });

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

    glm::vec4 outerHboxColor(0.2f, 0.3f, 0.8f, 1.0f);

    auto last_time = std::chrono::steady_clock::now();

    glfwSwapInterval(1);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);
//    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

//    hbox.addChild(&t);

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

//    FT_Library ft;
//    // All functions return a value different than 0 whenever an error occurred
//    if (FT_Init_FreeType(&ft)) {
//        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
//    }
//
//    // load font as face
//    FT_Face face;
//    if (FT_New_Face(ft, "../resources/font/ubuntu.ttf", 0, &face)) {
//        throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
//    }
//
//    FT_Set_Pixel_Sizes(face, 0, 96);
////    FT_Set_Char_Size(face, 0, 50, 96, 96);
//
//    unsigned int atlasWidth = 0;
//    unsigned int atlasHeight = 0;
//
//    for (int i = 32; i < 128; i++) {
//        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
//            std::cout << "Failed to load character " << (char) i << "." << std::endl;
//            continue;
//        }
//
//        atlasWidth += face->glyph->bitmap.width;
//        atlasHeight = std::max(atlasHeight, face->glyph->bitmap.rows);
//    }
//
//    unsigned int ubuntuAtlasId;
//    glEnable(GL_TEXTURE_2D);
//    glActiveTexture(GL_TEXTURE0);
//    glGenTextures(1, &ubuntuAtlasId);
//    glBindTexture(GL_TEXTURE_2D, ubuntuAtlasId);
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//    unsigned int w = 0;
//    unsigned int h = 0;
//
//    for(int i = 32; i < 128; i++) {
//        if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
//            fprintf(stderr, "Loading character %c failed!\n", i);
//            continue;
//        }
//
//        w += face->glyph->bitmap.width;
//        h = std::max(h, face->glyph->bitmap.rows);
//    }
//
//    // Setup texture
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    unsigned int x = 0;
//    for(int i = 32; i < 128; i++) {
//        if(FT_Load_Char(face, i, FT_LOAD_RENDER))
//            continue;
//
//        glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
//
//        x += face->glyph->bitmap.width;
//    }

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
//        std::pair<int, int> id(i, (int) a);  // area
        std::pair<int, int> id(i, (int) face->glyph->bitmap.width);  // width
//        std::pair<int, int> id(i, face->glyph->bitmap.rows);  // height
        ids.push_back(id);
        area += a;
    }

    std::sort(ids.begin(), ids.end(), sortIds);

    int atlasWidth = std::sqrt(area);
//    atlasWidth *= 1.06;
//    atlasWidth *= 0.58;  // With no retry
//    atlasWidth *= 0.535; // With no specified order
    atlasWidth *= 0.535; // With retry in rest
    int atlasHeight = atlasWidth;

    unsigned int ubuntuAtlasId;
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &ubuntuAtlasId);
    glBindTexture(GL_TEXTURE_2D, ubuntuAtlasId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//    std::vector<unsigned char> blank;
//    blank.reserve(atlasWidth * atlasHeight);
//    for (int i = 0; i < atlasWidth * atlasHeight; i++)
//        blank.push_back(0);

//    // Setup texture
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Draw characters
    int channels = CHANNELS;
    freeRectangles[0].push_back({0, 0, atlasWidth, atlasHeight});
    freeRectangles[1].push_back({0, 0, atlasWidth, atlasHeight});
    freeRectangles[2].push_back({0, 0, atlasWidth, atlasHeight});
    freeRectangles[3].push_back({0, 0, atlasWidth, atlasHeight});

//    int i = 32;

    auto* image = new unsigned char[atlasWidth * atlasHeight * CHANNELS]{0};
//    for (int i = 0; i < atlasWidth * atlasHeight * 3; i+=3)
//        image[i] = 255;

    for (int i = 0; i < ids.size(); i++) {
        auto p = ids[i];
        std::cout << p.first << std::endl;
        if (FT_Load_Char(face, p.first, FT_LOAD_RENDER))
            continue;

        int gw = (int) face->glyph->bitmap.width;
        int gh = (int) face->glyph->bitmap.rows;

        // Must insert into current channel
//        int bestFit = findBestCut(i % channels, gw, gh);
//        if (bestFit == -1)
//            throw std::runtime_error("Cannot pack texture in the given size!");
//
//        auto r = freeRectangles[i % channels][bestFit];
//        freeRectangles[i % channels].erase(freeRectangles[i % channels].begin() + bestFit);
//        auto[sm, lg] = insert(r, gw, gh);
//        freeRectangles[i % channels].push_back(sm);
//        freeRectangles[i % channels].push_back(lg);

        // Insert into any
        auto [ch, r] = tryInsertAny(gw, gh);

//        std::vector<unsigned char> idata;
//        idata.reserve(gw * gh);
//        for (int j = 0; j < gw * gh; j++)
//            idata.push_back(255);


        myTexSubImage2D(image, atlasWidth, ch, r.x, r.y, gw, gh, face->glyph->bitmap.buffer);
//        myTexSubImage2D(image, atlasWidth, i % channels, r.x, r.y, gw, gh, &idata[0]);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, r.x, r.y, gw, gh, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

//        glTexSubImage2D(GL_TEXTURE_2D, 0, r.x, r.y, gw, gh, GL_RED, GL_UNSIGNED_BYTE, &idata[0]);
    }

    // Setup texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] image;

//    for (int i = 32; i < 128; i++) {
//
//    }

    while (!window->shouldClose()) {
        auto now_time = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> delta = now_time - last_time;
        auto deltaCount = delta.count();
        float fps = 1000.0f / (float) deltaCount;
        last_time = now_time;

        glClearColor(0.19f, 0.19f, 0.19f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        InstancedRenderer::Render(editor->getViewport()->getProjectionViewMatrix());



        // Draw glyphs to atlas
//        char* pixel


////        FT_Set_Pixel_Sizes(face, 0, 100);
//
//        unsigned int atlasWidth = 0;
//        unsigned int atlasHeight = 0;
//
//        for (int i = 32; i < 128; i++) {
//            if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
//                std::cout << "Failed to load character " << (char) i << "." << std::endl;
//                continue;
//            }
//
//            atlasWidth += face->glyph->bitmap.width;
//            atlasHeight = std::max(atlasHeight, face->glyph->bitmap.rows);
//        }
//

//        // Create empty texture
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
//
//        // Fill texture with glyphs
//        unsigned int x = 0;
//        for (int i = 32; i < 128; i++) {
//            if (FT_Load_Char(face, i, FT_LOAD_RENDER))
//                continue;
//
//            glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
//
//            x += face->glyph->bitmap.width;
//        }



//        auto *font = FontHelper::GetFont("ubuntu");
//        auto it = font->find('a');
//        FTChar c = it->second;

        static int ch = 0;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ubuntuAtlasId);

        fontShader.use();
        fontShader.setMat4("projection_view", editor->getViewport()->getProjectionViewMatrix());
        fontShader.setInt("font_tex", 0);
        fontShader.setInt("channel", ch);

        fontVao->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

        // Rendering Setup
//        glm::mat4 view = glm::mat4(1.0f);
//        view = glm::translate(view, glm::vec3(editor->viewport.x, editor->viewport.y, 10.0f));
//        view = glm::inverse(view);

//        glm::mat4 proj;
//        proj = glm::ortho(0.0f, editor->viewport.width, 0.0f, editor->viewport.height, 0.0f, 100.0f);

//        Renderer::NewFrame(editor->getViewport()->getProjectionViewMatrix());

//        hbox.setBackground(outerHboxColor);
//        hbox.layout();
//        hbox.render();


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
            ImGui::Image((void *) (GLuint) ubuntuAtlasId, ImVec2((float) atlasWidth, (float) atlasHeight));

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
