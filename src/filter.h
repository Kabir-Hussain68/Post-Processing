#pragma once

#include <string>
#include <vector>

#include "shaderClass.h"
#include "framebuffer.h"

struct Param
{
    enum class Type { Float, Int };

    std::string name;   // Name of the uniform
    Type type;
    float value;        // Values are passed as floats but cast as int when passing
    float min, max;
};

class Filter
{
public:
    Filter(std::string name, const char* vertPath, const char* fragPath, std::vector<Param> params);
    virtual ~Filter() = default;

    const std::string& name() const { return m_Name; }

    // Draws one slider per param. Returns true if any value changed.
    bool drawSettings();

    // Uploads the uniforms and draws the quad into the 'out' framebyffer
    // Then the frambuffer is unbinded
    virtual void apply(unsigned int inputTex, FrameBuffer& out, unsigned int vao);

protected:
    void uploadParams();

    std::string m_Name;
    Shader m_Shader;
    std::vector<Param> m_Params;
};