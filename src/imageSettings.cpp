#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "imageSettings.h"

#include <glad/glad.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <vector>

#include "stb_image.h"
#include "stb_image_write.h"

Texture loadTexture(const std::string& path)
{
    Texture tex;
    tex.name = std::filesystem::path(path).stem().string(); 
    int nrChannels;

    unsigned char* data = stbi_load(path.c_str(), &tex.width, &tex.height, &nrChannels, 0);

    if (!data)
    {
        std::cout << "Failed to load texture: " << path << "\n";
        tex.width = 1;
        tex.height = 1;
        return tex;
    }

    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    return tex;
}

std::string exportImage(FrameBuffer& fb, const std::string& imageName, const std::string& filterName)
{
    namespace fs = std::filesystem;

    // Create Directory if it dont exist
    fs::path dir = fs::path(PROJECT_DIR) / "export";
    fs::create_directories(dir);

    // Adds "_" instead of " "
    std::string safeFilter = filterName;
    std::replace(safeFilter.begin(), safeFilter.end(), ' ', '_');

    // Original image name + filter name, e.g. chiken_DOG_Filter
    std::string baseName = imageName + "_" + safeFilter;

    // Adds (n) if image already exists
    fs::path file = dir / (baseName + ".png");
    for (int i = 1; fs::exists(file); i++)
    {
        file = dir / (baseName + " (" + std::to_string(i) + ").png");
    }

    std::vector<unsigned char> pixels = fb.readPixels();

    stbi_write_png(file.string().c_str(), fb.getWidth(), fb.getHeight(), 4, pixels.data(), fb.getWidth() * 4);

    return file.string();
}