
// preprocessor directives, tells compiler to only include if hasnt been included yet (even if multiple files include shader header)
#ifndef SHADER_H 
#define SHADER_H

#include <glad/glad.h> // include glad to get all required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader
{
public:
	// the shader program ID
	unsigned int ID;

	// constructor reads and builds the shader
	// file paths of source code for vertex and fragment shaders
	Shader(const char* vertexPath, const char* fragmentPath);
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat(const std::string& name, glm::mat4 matrix);

};

#endif