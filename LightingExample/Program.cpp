#include <stdexcept>
#include <iostream>
#include "Program.h"
#include "Utils.h"
#include "GL/glew.h"

Shader Shader::Load(ShaderFile const& shaderFile)
{
	return Shader(ReadFile(shaderFile.fileName), shaderFile.shaderType);
}

Shader::Shader(std::string const& sourceCode, GLenum shaderType)
	: rawShader(glCreateShader(shaderType))
{
	CheckGLError();
	if (rawShader == 0)
		throw std::runtime_error("Error creating shader");
	auto const cStr = sourceCode.c_str();
	glShaderSource(rawShader, 1, &cStr, nullptr);
	CheckGLError();
	Compile();
}

Shader::Shader(Shader&& other)
	: rawShader(other.rawShader)
{
	other.rawShader = 0;
}

Shader& Shader::operator=(Shader&& other)
{
	rawShader = other.rawShader;
	other.rawShader = 0;
	return *this;
}

Shader::~Shader()
{
	if (rawShader != 0)
		glDeleteShader(rawShader);
}

void Shader::Attach(GLuint rawProgram)
{
	glAttachShader(rawProgram, rawShader);
	CheckGLError();
}

void Shader::Compile()
{
	glCompileShader(rawShader);
	if (!GetCompiled())
		throw std::runtime_error("Shader failed to compile. " + GetLog());
}

GLint Shader::Get(GLenum parameter)
{
	GLint result;
	glGetShaderiv(rawShader, parameter, &result);
	return result;
}

GLint Shader::GetCompiled()
{
	return Get(GL_COMPILE_STATUS);
}

GLint Shader::GetLogLength()
{
	return Get(GL_INFO_LOG_LENGTH);
}

std::string Shader::GetLog()
{
	std::string log(GetLogLength() - 1, '\0');
	glGetShaderInfoLog(rawShader, log.size(), nullptr, log.data());
	return log;
}

std::vector<Shader> LoadShaders(std::vector<ShaderFile> const& shaderFiles)
{
	std::vector<Shader> shaders;
	for (auto const& shaderFile : shaderFiles)
		shaders.push_back(Shader::Load(shaderFile));
	return shaders;
}

Program::Program(std::vector<ShaderFile> const& shaderFiles)
	: rawProgram(glCreateProgram())
{
	if (rawProgram == 0)
		throw std::runtime_error("Error creating program");
	auto shaders = LoadShaders(shaderFiles);
	for (auto& shader : shaders)
		shader.Attach(rawProgram);
	Link();
}

Program::~Program()
{
	glDeleteProgram(rawProgram);
}

void Program::Use()
{
	glUseProgram(rawProgram);
	CheckGLError();
}

GLint Program::GetUniformLocation(std::string const& uniformName)
{
	GLint const result = glGetUniformLocation(rawProgram, uniformName.c_str());
	if (result == -1)
		throw std::runtime_error("Invalid uniform name");
	CheckGLError();
	return result;
}

void Program::Link()
{
	glLinkProgram(rawProgram);
	if (!GetLinked())
		throw std::runtime_error("Shader failed to link. " + GetLog());
}

GLint Program::Get(GLenum parameter)
{
	GLint result;
	glGetProgramiv(rawProgram, parameter, &result);
	return result;
}

GLint Program::GetLinked()
{
	return Get(GL_LINK_STATUS);
}

GLint Program::GetLogLength()
{
	return Get(GL_INFO_LOG_LENGTH);
}

std::string Program::GetLog()
{
	std::string log(GetLogLength() - 1, '\0');
	glGetProgramInfoLog(rawProgram, log.size(), nullptr, log.data());
	return log;
}