#include "Shader.h"
#include <GL/glew.h> //Loads opengl from driver, simpler than glad

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include <fstream>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filepath) 
	: m_FilePath(filepath), m_RendererID(0) //initializer list
{
	//Load shaders from file
	ShaderProgramSource source = ParseShader();
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(m_RendererID);
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_RendererID)); //clean up
}

ShaderProgramSource Shader::ParseShader() {
	//Goes through shader file and parses out different shaders

	//Could do it faster using C
	std::ifstream stream(m_FilePath);

	enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			//If shader is found
			if (line.find("vertex") != std::string::npos) {
				//set mode to vertex
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				//set mode to fragment
				type = ShaderType::FRAGMENT;
			}
			else {
				std::cout << "Error parsing shader, type not implemented" << std::endl;
			}
		}
		else {
			//Add to appropriate part of array
			ss[(int)type] << line << '\n';
		}
	}

	//return struct
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned id = glCreateShader(type);
	const char* src = source.c_str(); //get raw string
	glShaderSource(id, 1, &src, nullptr); //id, number of sources, address of ptr, length
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //get id of compile of result
	if (result == GL_FALSE) {
		//did not compile sucessfully
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char)); //dynamic allocation
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Fail to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	/*generate shader
	takes the source code as input as strings (simple way)
	returns identifier int*/

	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//Attach shaders
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	//cleanup
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void Shader::Bind() const{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const{
	GLCall(glUseProgram(0));
}

/********Various uniform functions*********/
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	int location = GetUniformLocation(name);
	GLCall(glUniform4f(location, v0, v1, v2, v3))
}

void Shader::SetUniform1f(const std::string& name, float v0) {
	int location = GetUniformLocation(name);
	GLCall(glUniform1f(location, v0))
}

void Shader::SetUniform1i(const std::string& name, int v0) {
	int location = GetUniformLocation(name);
	GLCall(glUniform1i(location, v0))
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	int location = GetUniformLocation(name);
	//Location, Count, Transpose?, pointer to start of matrix
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]))
}

int Shader::GetUniformLocation(const std::string& name) {
	//Gets uniform location and caches location
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		//if cache contains name already
		return m_UniformLocationCache[name];
	}

	int location = glGetUniformLocation(m_RendererID, name.c_str()); //get location from shader, in 4.3 can set location
	if (location == -1) { //check for location unused in memory
		std::cout << "Warning uniform " << name << " not found in memory" << std::endl;
	}
	//add name:location to cache
	m_UniformLocationCache[name] = location;

	return location;
}