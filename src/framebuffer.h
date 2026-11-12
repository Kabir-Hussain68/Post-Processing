#pragma once

#include <cstdint>
#include <vector>

class FrameBuffer
{
public:
    FrameBuffer(uint32_t width, uint32_t height);
    ~FrameBuffer();

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;

    void bind();
    void unBind(int restoreW, int restoreH);
    void resize(uint32_t width, uint32_t height);

    std::vector<unsigned char> readPixels() const;

    uint32_t getColorAttachment() const { return m_ColorAttachment; }
    uint32_t getWidth() const { return m_Width; }
    uint32_t getHeight() const { return m_Height; }

private:
    void invalidate();

    uint32_t m_RendererID = 0;
    uint32_t m_ColorAttachment = 0;
    uint32_t m_Width, m_Height;
};