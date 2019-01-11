#pragma once

#include <vector>
#include "GL/glew.h"
#include "Utils.h"

class Buffers
{
public:
	static Buffers ForTargets(std::vector<GLenum> const& targets);

	explicit Buffers(GLsizei count);

	Buffers() = default;

	~Buffers();

	Buffers(Buffers const& other) = delete;

	Buffers& operator=(Buffers const& other) = delete;

	Buffers(Buffers&& other);

	Buffers& operator=(Buffers&& other);

	void Bind(GLenum target, std::size_t index);

	void BindBase(GLenum target, GLuint base, std::size_t index);

private:
	void Delete();

	std::vector<GLuint> rawBuffers;
};

template <class Container>
void BufferData(GLenum target, Container const& container, GLenum usage)
{
	glBufferData(target, sizeof(typename Container::value_type) * container.size(), container.data(), usage);
	CheckGLError();
}

void BufferData(GLenum target, GLsizeiptr size, GLenum usage);

template <class Container>
void BufferSubData(GLenum target, GLintptr offset, Container const& container)
{
	glBufferSubData(target, offset, sizeof(typename Container::value_type) * container.size(), container.data());
}
