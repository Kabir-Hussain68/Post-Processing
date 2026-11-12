#pragma once

#include <string>
#include "framebuffer.h"

struct Texture
{
    unsigned int id = 0;   // 0 means the load failed
    int width = 1;
    int height = 1;
    std::string name;   // Name of the file loaded
};

// Loads an image file
Texture loadTexture(const std::string& path);

// Exports the image and returns the path of the saved file
std::string exportImage(FrameBuffer& fb, const std::string& imageName, const std::string& filterName);