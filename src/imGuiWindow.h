#pragma once

struct GLFWwindow;

class ImGuiHelper
{
private:
    bool m_startDockSpace = false;

public:
    void init(GLFWwindow* window);
    void shutdown();

    void begin();
    void end();

    void beginDockSpace();
};