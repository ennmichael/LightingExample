#pragma once

#include <vector>
#include <string>
#include "GL/glew.h"

// TODO Uniforms

struct ShaderFile {
	std::string fileName;
	GLenum shaderType;
};

class Shader {
public:
	static Shader Load(ShaderFile const& shaderFile);

	explicit Shader(std::string const& sourceCode, GLenum shaderType);

	~Shader();

	Shader(Shader const& other) = delete;

	Shader(Shader&& other);

	Shader& operator=(Shader const& other) = delete;

	Shader& operator=(Shader&& other);

	void Attach(GLuint rawProgram);

private:
	void Compile();

	GLint Get(GLenum parameter);

	GLint GetCompiled();

	GLint GetLogLength();

	std::string GetLog();

	GLuint rawShader;
};

std::vector<Shader> LoadShaders(std::vector<ShaderFile> const& shaderFiles);

class Program {
public:
	explicit Program(std::vector<ShaderFile> const& shaderFiles);

	~Program();

	Program(Program const& other) = delete;

	Program(Program&& other) = delete;

	Program& operator=(Program const& other) = delete;

	Program& operator=(Program&& other) = delete;

	void Use();

	GLint GetUniformLocation(std::string const& uniformName);

private:
	void Link();

	GLint Get(GLenum parameter);

	GLint GetLinked();

	GLint GetLogLength();

	std::string GetLog();

	GLuint rawProgram = 0;
};
