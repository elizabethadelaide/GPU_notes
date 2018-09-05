#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	std::string m_FilePath; //remember the filepath
	unsigned int m_RendererID;
	//Caching for uniforms
	//(Thought: With proper consting this would be mutable?)...
	std::unordered_map<std::string, unsigned int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const; //use program
	void Unbind() const;

	//Set uniforms:
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3); //simple 4float uniform 4now
	void SetUniform1f(const std::string& name, float v0); //simple 4float uniform 4now
	void SetUniform1i(const std::string& name, int v0);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	ShaderProgramSource ParseShader();
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};