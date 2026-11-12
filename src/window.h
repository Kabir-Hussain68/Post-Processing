#pragma once

#include "GLFW/glfw3.h"

#include <iostream>
#include <string>


class Window
{
private:
    GLFWwindow* m_Window;

    struct windowData
    {
        std::string title;
        unsigned int width;
        unsigned int height;
        bool Vsync;
    };

    windowData m_Data;

public:
    struct windowProps
    {
        std::string title;
        uint32_t width;
        uint32_t height;

        windowProps(const std::string& title = "Fahren Engine", 
                    uint32_t width = 1920,
                    uint32_t height = 1080 )
            : title(title), width(width), height(height)    {}
    };

    Window(const windowProps& props);
    virtual ~Window();

    void onUpdate();

    inline unsigned int getWidth() const { return m_Data.width; }
    inline unsigned int getHeight() const { return m_Data.height; }

    void setVsync(bool enabled);
    bool isVsync() const;

    inline virtual void* getNativeWindow() const { return m_Window; };

    bool windowShouldClose() const  { return glfwWindowShouldClose(m_Window); };

private:
    virtual void Init(const windowProps& props);
    virtual void shutdown();
};