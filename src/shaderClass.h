#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>

#include<unordered_map>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
private:
	std::unordered_map<std::string, int> m_UniformLocationCache;

	int getLocation(const std::string& name);
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	void setInt(const std::string& name, const int value);
	void setFloat(const std::string& name, const float value);
	void setFloat2(const std::string& name, const float x, const float y);
	void setFloat3(const std::string& name, const float x, const float y, const float z);
};
#endif