#include "filter.h"

#include <glad/glad.h>
#include <imgui.h>

Filter::Filter(std::string name, const char* vertPath, const char* fragPath, std::vector<Param> params)
    : m_Name(std::move(name)), m_Shader(vertPath, fragPath), m_Params(std::move(params))
{
}

bool Filter::drawSettings()
{
    bool changed = false;

    for (auto& p : m_Params)
    {
        if (p.type == Param::Type::Float)
        {
            changed |= ImGui::SliderFloat(p.name.c_str(), &p.value, p.min, p.max);
        }
        else
        {
            int v = static_cast<int>(p.value);
            if (ImGui::SliderInt(p.name.c_str(), &v, static_cast<int>(p.min), static_cast<int>(p.max)))
            {
                p.value = static_cast<float>(v);
                changed = true;
            }
        }
    }

    return changed;
}

void Filter::uploadParams()
{
    for (auto& p : m_Params)
    {
        if (p.type == Param::Type::Float)
            m_Shader.setFloat(p.name, p.value);
        else
            m_Shader.setInt(p.name, static_cast<int>(p.value));
    }
}

void Filter::apply(unsigned int inputTex, FrameBuffer& out, unsigned int vao)
{
    out.bind();

    m_Shader.Activate(); 
    m_Shader.setInt("imageTexture", 0);
    m_Shader.setFloat2("texelSize", 1.0f / out.getWidth(), 1.0f / out.getHeight());
    uploadParams();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTex);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}