#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include "window.h"
#include "imGuiWindow.h"
#include "filter.h"
#include "framebuffer.h"
#include "imageSettings.h"

// Screen dimensions
const unsigned int screenWidth = 1920;
const unsigned int screenHeight = 1080;

float vertices[] = {
    // Positions           // TexCoords
    -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,    1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,    0.0f, 1.0f,
     1.0f,  1.0f, 0.0f,    1.0f, 1.0f
};

int main()
{
    Window window(Window::windowProps("Post Processing", screenWidth, screenHeight));
    GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window.getNativeWindow());

    // ImGui
    ImGuiHelper imgui;
    imgui.init(glfwWindow);

    // Filters created after window for GL context and reserves avoid allocation when adding
    std::vector<Filter> filters;
    filters.reserve(8);

    // All the params for all the filters (a lil scuffed but works)
    filters.emplace_back("DOG Filter(Basic)", "assets/shaders/default.vert", "assets/shaders/default.frag",
        std::vector<Param>{
            { "sigma", Param::Type::Float, 2.5f, 0.5f, 10.0f },
            { "k", Param::Type::Float, 1.6f, 1.0f, 3.0f },
            { "threshold", Param::Type::Float, 0.012f, 0.0f, 0.05f }
        });

    // -1 because no filter is chosen so none
    int current = -1;

    // Path of the last exported file
    std::string lastExport;

    // Setup VAO and VBO
    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Tex coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load the texture
    Texture image = loadTexture("assets/textures/chicken.jpg");

    // Framebuffer
    FrameBuffer fb(image.width, image.height);

    // Only re-run the filter when something changes
    bool dirty = true;

    // Render loop
    while (!window.windowShouldClose())
    {
        // Framebuffer initialization
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(glfwWindow, &fbWidth, &fbHeight);

        // Filter pass over the image
        if (dirty && current >= 0)
        {
            filters[current].apply(image.id, fb, VAO);
            fb.unBind(fbWidth, fbHeight);
            dirty = false;
        }

        // Screen
        glViewport(0, 0, fbWidth, fbHeight);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ImGui
        imgui.begin();
        imgui.beginDockSpace();

        // Image window
        ImGui::Begin("Image");

        // For preserving aspect ration
        ImVec2 availableRegion = ImGui::GetContentRegionAvail();
        float scale = std::min(availableRegion.x / fb.getWidth(), availableRegion.y / fb.getHeight());
        ImVec2 size(fb.getWidth() * scale, fb.getHeight() * scale);

        // Center the image
        ImVec2 cursor = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(cursor.x + (availableRegion.x - size.x) * 0.5f, cursor.y + (availableRegion.y - size.y) * 0.5f));

        if (size.x > 0 && size.y > 0)
        {
            // Displaying the image
            unsigned int displayTex = (current >= 0) ? fb.getColorAttachment() : image.id;
            ImGui::Image((ImTextureID)(intptr_t)displayTex, size);
        }

        ImGui::End();

        // Settings window
        ImGui::Begin("Settings");

        if (current < 0)
        {
            // Holding text for the drop down menu to select a filter
            ImGui::TextUnformatted("Select a filter to begin");

            if (ImGui::BeginCombo("Filter", "None"))
            {
                for (int i = 0; i < (int)filters.size(); i++)
                {
                    if (ImGui::Selectable(filters[i].name().c_str(), false))
                    {
                        current = i;
                        dirty = true;
                    }
                }
                ImGui::EndCombo();
            }
        }
        else
        {
            // Disable the drop down and show params for the filter chosen
            ImGui::Text("Filter: %s", filters[current].name().c_str());
            ImGui::Separator();

            if (filters[current].drawSettings())
            {
                dirty = true;
            }

            ImGui::Separator();

            // Export button
            if (ImGui::Button("Export"))
            {
                lastExport = exportImage(fb, image.name, filters[current].name());
            }

            if (!lastExport.empty())
            {
                ImGui::TextWrapped("Saved: %s", lastExport.c_str());
            }
        }

        ImGui::End();

        imgui.end();

        window.onUpdate();
    }

    // Cleanup
    imgui.shutdown();

    glDeleteTextures(1, &image.id);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}