#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include <GL/glew.h>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	int ColorLocation();

	void Recompile(const char* vertexFile, const char* fragmentFile);
};
#endif