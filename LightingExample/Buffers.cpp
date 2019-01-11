#include <utility>
#include "Buffers.h"
#include "Utils.h"

Buffers Buffers::ForTargets(std::vector<GLenum> const& targets)
{
	Buffers buffers(targets.size());
	for (std::size_t i = 0; i < targets.size(); ++i)
		buffers.Bind(targets.at(i), i);
	return buffers;
}

Buffers::Buffers(GLsizei count)
	: rawBuffers(count)
{
	glCreateBuffers(rawBuffers.size(), rawBuffers.data());
}

Buffers::~Buffers()
{
	Delete();
}

Buffers::Buffers(Buffers&& other)
	: rawBuffers(std::move(other.rawBuffers))
{
	other.rawBuffers = {};
}

Buffers& Buffers::operator=(Buffers&& other)
{
	Delete();
	rawBuffers = std::move(other.rawBuffers);
	other.rawBuffers = {};
	return *this;
}

void Buffers::Bind(GLenum target, std::size_t index)
{
	glBindBuffer(target, rawBuffers.at(index));
}

void Buffers::BindBase(GLenum target, GLuint base, std::size_t index)
{
	glBindBufferBase(target, base, rawBuffers.at(index));
	CheckGLError();
}

void Buffers::Delete()
{
	if (rawBuffers.size() == 0)
		return;
	glDeleteBuffers(rawBuffers.size(), rawBuffers.data());
}

void BufferData(GLenum target, GLsizeiptr size, GLenum usage)
{
	glBufferData(target, size, nullptr, usage);
	CheckGLError();
}
