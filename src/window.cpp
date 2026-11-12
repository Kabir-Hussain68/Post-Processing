#include "window.h"

#include <glad/glad.h>

static bool m_GLFWInitialized = false;


Window::Window(const windowProps& props)
{
    Init(props);
}

Window::~Window()
{
    shutdown();
}

void Window::Init(const windowProps& props)
{
    m_Data.title = props.title;
    m_Data.width = props.width;
    m_Data.height = props.height;

    if (!m_GLFWInitialized)
    {
        glfwInit();
        m_GLFWInitialized = true;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);

    if (!m_Window)
    {
        std::cout<<"Window Not initialized";
    }

    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<"Glad failed to load";
        glfwTerminate();
    }

    glfwSetWindowUserPointer(m_Window, &m_Data);
    setVsync(true);

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
    {
        windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
        data.width = width;
        data.height = height;
    });
}

void Window::shutdown()
{
    glfwDestroyWindow(m_Window);
}

void Window::onUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

void Window::setVsync(bool enabled)
{
    if(enabled)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }

    m_Data.Vsync = enabled;
}

bool Window::isVsync() const
{
    return m_Data.Vsync;
}